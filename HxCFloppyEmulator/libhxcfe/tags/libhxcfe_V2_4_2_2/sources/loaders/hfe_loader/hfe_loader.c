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
// File : HFE_DiskFile.c
// Contains: HFE floppy image loader
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "libhxcfe.h"

#include "floppy_loader.h"
#include "floppy_utils.h"

#include "hfe_loader.h"
#include "hfe_format.h"

#include "libhxcadaptor.h"

extern unsigned char bit_inverter[];

char * trackencodingcode[]=
{
	"ISOIBM_MFM_ENCODING",
	"AMIGA_MFM_ENCODING",
	"ISOIBM_FM_ENCODING",
	"EMU_FM_ENCODING",
	"UNKNOWN_ENCODING"
};


char * interfacemodecode[]=
{
	"IBMPC_DD_FLOPPYMODE",
	"IBMPC_HD_FLOPPYMODE",
	"ATARIST_DD_FLOPPYMODE",
	"ATARIST_HD_FLOPPYMODE",
	"AMIGA_DD_FLOPPYMODE",
	"AMIGA_HD_FLOPPYMODE",
	"CPC_DD_FLOPPYMODE",
	"GENERIC_SHUGART_DD_FLOPPYMODE",
	"IBMPC_ED_FLOPPYMODE",
	"MSX2_DD_FLOPPYMODE",
	"C64_DD_FLOPPYMODE",
	"EMU_SHUGART_FLOPPYMODE"
};

int HFE_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	FILE *f;
	picfileformatheader header;

	floppycontext->hxc_printf(MSG_DEBUG,"HFE_libIsValidDiskFile");

	if(imgfile)
	{
		f=hxc_fopen(imgfile,"rb");
		if(f==NULL)
		{
			return HXCFE_ACCESSERROR;
		}
		fread(&header,sizeof(header),1,f);
		hxc_fclose(f);

		if( !strncmp((char*)header.HEADERSIGNATURE,"HXCPICFE",8))
		{
			floppycontext->hxc_printf(MSG_DEBUG,"HFE_libIsValidDiskFile : HFE file !");
			return HXCFE_VALIDFILE;
		}
		else
		{
			floppycontext->hxc_printf(MSG_DEBUG,"HFE_libIsValidDiskFile : non HFE file !");
			return HXCFE_BADFILE;
		}
	}
	else
	{
		floppycontext->hxc_printf(MSG_DEBUG,"HFE_libIsValidDiskFile : non HFE file !");
		return HXCFE_BADFILE;
	}

	return HXCFE_BADPARAMETER;
}

int HFE_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{
	FILE * f;
	picfileformatheader header;
	unsigned int i,j,k,l,offset,offset2;
	CYLINDER* currentcylinder;
	SIDE* currentside;
    pictrack* trackoffsetlist;
    unsigned int tracks_base;
    unsigned char * hfetrack;
	unsigned int nbofblock,tracklen;


	floppycontext->hxc_printf(MSG_DEBUG,"HFE_libLoad_DiskFile %s",imgfile);

	f=hxc_fopen(imgfile,"rb");
	if(f==NULL)
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}

	fread(&header,sizeof(header),1,f);

	if(!strncmp((char*)header.HEADERSIGNATURE,"HXCPICFE",8))
	{

		floppydisk->floppyNumberOfTrack=header.number_of_track;
		floppydisk->floppyNumberOfSide=header.number_of_side;
		floppydisk->floppyBitRate=header.bitRate*1000;
		floppydisk->floppySectorPerTrack=-1;
		floppydisk->floppyiftype=header.floppyinterfacemode;


		floppycontext->hxc_printf(MSG_DEBUG,"HFE File : %d track, %d side, %d bit/s, %d sectors, interface mode %s, track encoding:%s",
			floppydisk->floppyNumberOfTrack,
			floppydisk->floppyNumberOfSide,
			floppydisk->floppyBitRate,
			floppydisk->floppySectorPerTrack,
			floppydisk->floppyiftype<0xC?interfacemodecode[floppydisk->floppyiftype]:"Unknow!",
			(header.track_encoding&(~3))?trackencodingcode[4]:trackencodingcode[header.track_encoding&0x3]);

        trackoffsetlist=(pictrack*)malloc(sizeof(pictrack)* header.number_of_track);
        memset(trackoffsetlist,0,sizeof(pictrack)* header.number_of_track);
        fseek( f,512,SEEK_SET);
        fread( trackoffsetlist,sizeof(pictrack)* header.number_of_track,1,f);

        tracks_base= 512+( (((sizeof(pictrack)* header.number_of_track)/512)+1)*512);
        fseek( f,tracks_base,SEEK_SET);

		floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);
		memset(floppydisk->tracks,0,sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);

		for(i=0;i<floppydisk->floppyNumberOfTrack;i++)
		{

			fseek(f,(trackoffsetlist[i].offset*512),SEEK_SET);
			if(trackoffsetlist[i].track_len&0x1FF)
			{
				tracklen=(trackoffsetlist[i].track_len&(~0x1FF))+0x200;
			}
			else
			{
				tracklen=trackoffsetlist[i].track_len;
			}

			hfetrack=(unsigned char*)malloc( tracklen );

			floppycontext->hxc_printf(MSG_DEBUG,"HFE File : reading track %d, track size:%d - file offset:%.8X",
				i,tracklen,(trackoffsetlist[i].offset*512));

			fread( hfetrack,tracklen,1,f);


			floppydisk->tracks[i]=(CYLINDER*)malloc(sizeof(CYLINDER));
			currentcylinder=floppydisk->tracks[i];
			currentcylinder->number_of_side=floppydisk->floppyNumberOfSide;
			currentcylinder->sides=(SIDE**)malloc(sizeof(SIDE*)*currentcylinder->number_of_side);
			memset(currentcylinder->sides,0,sizeof(SIDE*)*currentcylinder->number_of_side);
			currentcylinder->floppyRPM=header.floppyRPM;


		/*	floppycontext->hxc_printf(MSG_DEBUG,"read track %d side %d at offset 0x%x (0x%x bytes)",
			trackdesc.track_number,
			trackdesc.side_number,
			trackdesc.mfmtrackoffset,
			trackdesc.mfmtracksize);
*/
			for(j=0;j<currentcylinder->number_of_side;j++)
			{
				currentcylinder->sides[j]=malloc(sizeof(SIDE));
				memset(currentcylinder->sides[j],0,sizeof(SIDE));
				currentside=currentcylinder->sides[j];

				currentside->number_of_sector=floppydisk->floppySectorPerTrack;
				currentside->tracklen=tracklen/2;

				currentside->databuffer=malloc(currentside->tracklen);
				memset(currentside->databuffer,0,currentside->tracklen);

				currentside->flakybitsbuffer=0;

				currentside->indexbuffer=malloc(currentside->tracklen);
				memset(currentside->indexbuffer,0,currentside->tracklen);

				for(k=0;k<256;k++)
				{
					currentside->indexbuffer[k]=0xFF;
				}

				currentside->timingbuffer=0;
				currentside->bitrate=floppydisk->floppyBitRate;

				currentside->track_encoding=header.track_encoding;

				if( i == 0 )
				{
					if ( j == 0 )
					{
						if(!header.track0s0_altencoding)
						{
							currentside->track_encoding = header.track0s0_encoding;
						}
					}
					else
					{
						if(!header.track0s1_altencoding)
						{
							currentside->track_encoding = header.track0s1_encoding;
						}
					}
				}


				nbofblock=(currentside->tracklen/256);
				for(k=0;k<nbofblock;k++)
				{
					for(l=0;l<256;l++)
					{
						offset=(k*256)+l;
						offset2=(k*512)+l+(256*j);
						currentside->databuffer[offset]=bit_inverter[hfetrack[offset2]];
					}
				}

				currentside->tracklen=currentside->tracklen*8;

			}

			free(hfetrack);
		}

		free(trackoffsetlist);

		hxc_fclose(f);
		return HXCFE_NOERROR;
	}

	hxc_fclose(f);
	floppycontext->hxc_printf(MSG_ERROR,"bad header");
	return HXCFE_BADFILE;
}

int HFE_libWrite_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppy,char * filename);

int HFE_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{

	static const char plug_id[]="HXC_HFE";
	static const char plug_desc[]="SD Card HxCFE HFE file Loader";
	static const char plug_ext[]="hfe";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	HFE_libIsValidDiskFile,
		(LOADDISKFILE)		HFE_libLoad_DiskFile,
		(WRITEDISKFILE)		HFE_libWrite_DiskFile,
		(GETPLUGININFOS)	HFE_libGetPluginInfo
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

int EXTHFE_libWrite_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppy,char * filename);

int EXTHFE_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{

	static const char plug_id[]="HXC_EXTHFE";
	static const char plug_desc[]="SD Card HxCFE EXTENDED HFE file Loader";
	static const char plug_ext[]="hfe";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	0,
		(LOADDISKFILE)		0,
		(WRITEDISKFILE)		EXTHFE_libWrite_DiskFile,
		(GETPLUGININFOS)	EXTHFE_libGetPluginInfo
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

int HFE_HDDD_A2_libWrite_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppy,char * filename);

int HFE_HDDD_A2_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{

	static const char plug_id[]="HXC_HDDD_A2_HFE";
	static const char plug_desc[]="SD Card HxCFE HFE file Loader (HDDD A2 Support)";
	static const char plug_ext[]="hfe";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	0,
		(LOADDISKFILE)		0,
		(WRITEDISKFILE)		HFE_HDDD_A2_libWrite_DiskFile,
		(GETPLUGININFOS)	HFE_HDDD_A2_libGetPluginInfo
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
