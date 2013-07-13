/*
//
// Copyright (C) 2006 - 2013 Jean-Fran�ois DEL NERO
//
// This file is part of the HxCFloppyEmulator library
//
// HxCFloppyEmulator may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// HxCFloppyEmulator is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// HxCFloppyEmulator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HxCFloppyEmulator; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-----------H----H--X----X-----CCCCC----22222----0000-----0000------11----------//
//----------H----H----X-X-----C--------------2---0----0---0----0--1--1-----------//
//---------HHHHHH-----X------C----------22222---0----0---0----0-----1------------//
//--------H----H----X--X----C----------2-------0----0---0----0-----1-------------//
//-------H----H---X-----X---CCCCC-----222222----0000-----0000----1111------------//
//-------------------------------------------------------------------------------//
//----------------------------------------------------- http://hxc2001.free.fr --//
///////////////////////////////////////////////////////////////////////////////////
// File : apple2_do_loader.c
// Contains: DO Apple II floppy image loader
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "libhxcfe.h"

#include "floppy_loader.h"
#include "floppy_utils.h"

#include "apple2_do_loader.h"

#include "libhxcadaptor.h"

int Apple2_do_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int filesize;

	floppycontext->hxc_printf(MSG_DEBUG,"Apple2_do_libIsValidDiskFile");

	if( hxc_checkfileext(imgfile,"do"))
	{
		filesize=hxc_getfilesize(imgfile);
		if(filesize<0)
		{
			floppycontext->hxc_printf(MSG_ERROR,"Apple2_do_libIsValidDiskFile : Cannot open %s !",imgfile);
			return HXCFE_ACCESSERROR;
		}

		if( filesize%(16*1*256) || ( (filesize/(16*1*256)) > 50 ) )
		{
			floppycontext->hxc_printf(MSG_DEBUG,"Apple2_do_libIsValidDiskFile : non DO file !");
			return HXCFE_BADFILE;
		}

		floppycontext->hxc_printf(MSG_DEBUG,"Apple2_do_libIsValidDiskFile : DO file !");
		return HXCFE_VALIDFILE;
	}

	floppycontext->hxc_printf(MSG_DEBUG,"Apple2_do_libIsValidDiskFile : non DO file !");
	return HXCFE_BADFILE;
}



int Apple2_do_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{

	FILE * f;
	unsigned int filesize;
	unsigned int i,j,k;
	unsigned int file_offset;
	unsigned char* trackdata;
	unsigned char trackformat;
	unsigned char gap3len,interleave;
	unsigned short sectorsize,rpm;
	unsigned int bitrate;

	SECTORCONFIG* sectorconfig;
	CYLINDER* currentcylinder;

	floppycontext->hxc_printf(MSG_DEBUG,"Apple2_do_libLoad_DiskFile %s",imgfile);

	f=hxc_fopen(imgfile,"rb");
	if(f==NULL)
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}

	fseek (f , 0 , SEEK_END);
	filesize=ftell(f);
	fseek (f , 0 , SEEK_SET);

	if(filesize!=0)
	{

			sectorsize=256;

            bitrate=250000;
            rpm=283;
            interleave=3;
            gap3len=16;
			trackformat=APPLE2_GCR6A2;
            floppydisk->floppyNumberOfSide=1;
            floppydisk->floppySectorPerTrack=16;
            floppydisk->floppyNumberOfTrack=filesize/( floppydisk->floppyNumberOfSide*floppydisk->floppySectorPerTrack*256);

			floppydisk->floppyBitRate=bitrate;
			floppydisk->floppyiftype=GENERIC_SHUGART_DD_FLOPPYMODE;
			floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);

			floppycontext->hxc_printf(MSG_DEBUG,"rpm %d bitrate:%d track:%d side:%d sector:%d",rpm,bitrate,floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack);

			sectorconfig=(SECTORCONFIG*)malloc(sizeof(SECTORCONFIG)*floppydisk->floppySectorPerTrack);
			memset(sectorconfig,0,sizeof(SECTORCONFIG)*floppydisk->floppySectorPerTrack);

			trackdata=(unsigned char*)malloc(sectorsize*floppydisk->floppySectorPerTrack);

			for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
			{

				floppydisk->tracks[j]=allocCylinderEntry(rpm,floppydisk->floppyNumberOfSide);
				currentcylinder=floppydisk->tracks[j];

				for(i=0;i<floppydisk->floppyNumberOfSide;i++)
				{

					file_offset=(sectorsize*(j*floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide))+
						(sectorsize*(floppydisk->floppySectorPerTrack)*i);

					fseek (f , file_offset , SEEK_SET);
					fread(trackdata,sectorsize*floppydisk->floppySectorPerTrack,1,f);

					memset(sectorconfig,0,sizeof(SECTORCONFIG)*floppydisk->floppySectorPerTrack);
					for(k=0;k<floppydisk->floppySectorPerTrack;k++)
					{
						sectorconfig[k].cylinder=j;
						sectorconfig[k].head=i;
						sectorconfig[k].sector=k;
						sectorconfig[k].bitrate=floppydisk->floppyBitRate;
						sectorconfig[k].gap3=gap3len;
						sectorconfig[k].sectorsize=sectorsize;
						sectorconfig[k].input_data=&trackdata[k*sectorsize];
						sectorconfig[k].trackencoding=trackformat;
					}

					currentcylinder->sides[i]=tg_generateTrackEx(floppydisk->floppySectorPerTrack,sectorconfig,interleave,0,floppydisk->floppyBitRate,rpm,trackformat,200,2500,-2500);
				}
			}

			free(sectorconfig);
			floppycontext->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");

			hxc_fclose(f);
			return HXCFE_NOERROR;
	}

	floppycontext->hxc_printf(MSG_ERROR,"file size=%d !?",filesize);
	hxc_fclose(f);
	return HXCFE_BADFILE;
}


int Apple2_do_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{

	static const char plug_id[]="APPLE2_DO";
	static const char plug_desc[]="Apple II DO Loader";
	static const char plug_ext[]="do";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	Apple2_do_libIsValidDiskFile,
		(LOADDISKFILE)		Apple2_do_libLoad_DiskFile,
		(WRITEDISKFILE)		0,
		(GETPLUGININFOS)	Apple2_do_libGetPluginInfo
	};

	return libGetPluginInfo(
			floppycontext,
			infotype,
			returnvalue,
			plug_id,
			plug_desc,
			&plug_funcs,
			plug_ext
			);
}
