/*
//
// Copyright (C) 2006, 2007, 2008, 2009 Jean-Fran?ois DEL NERO
//
// This file is part of HxCFloppyEmulator.
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
// File : sap_loader.c
// Contains: TO8D SAP floppy image loader and plugins interfaces
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "hxc_floppy_emulator.h"
#include "internal_floppy.h"
#include "floppy_loader.h"
#include "floppy_utils.h"

#include "../common/crc.h"
#include "../common/iso_ibm_track.h"
#include "../common/libs/libsap/libsap.h"
#include "sap_loader.h"

#include "../common/os_api.h"


int SAP_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int pathlen,floppyformat;
	char * filepath;
	sapID sapid;
	floppycontext->hxc_printf(MSG_DEBUG,"SAP_libIsValidDiskFile %s",imgfile);
	if(imgfile)
	{
		pathlen=strlen(imgfile);
		if(pathlen!=0)
		{
			filepath=malloc(pathlen+1);
			if(filepath!=0)
			{
				sprintf(filepath,"%s",imgfile);
				strlower(filepath);
				
				if(strstr( filepath,".sap" )!=NULL)
				{
					sapid=sap_OpenArchive(filepath, &floppyformat);
					if(sapid!=SAP_ERROR)
					{
						sap_CloseArchive(sapid);
						floppycontext->hxc_printf(MSG_DEBUG,"SAP file !");
						free(filepath);
						return LOADER_ISVALID;
					}
					else
					{
						floppycontext->hxc_printf(MSG_DEBUG,"non SAP file !");
						free(filepath);
						return LOADER_BADFILE;
					}
					
				}
				else
				{
					floppycontext->hxc_printf(MSG_DEBUG,"non SAP file !");
					free(filepath);
					return LOADER_BADFILE;
				}
			}
		}
	}
	
	return LOADER_BADPARAMETER;
}



int SAP_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{
	unsigned int i,j;
	char* trackdata;
	int tracklen;
	int gap3len,interleave,rpm;
	int sectorsize;

	int trackformat;
	int floppyformat;
	sapID sapid;

	CYLINDER* currentcylinder;
	SIDE* currentside;
	
	
	floppycontext->hxc_printf(MSG_DEBUG,"SAP_libLoad_DiskFile %s",imgfile);
	

	sapid=sap_OpenArchive(imgfile, &floppyformat);
	if(sapid==SAP_ERROR)
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return -1;
	}


	gap3len=50;
	interleave=1;

	switch(floppyformat)
	{

	case SAP_FORMAT1:
		sectorsize=SAP_SECTSIZE1;
		floppydisk->floppyNumberOfTrack=SAP_NTRACKS1;
		floppydisk->floppySectorPerTrack=SAP_NSECTS;
		floppydisk->floppyNumberOfSide=1;
		trackformat=ISOFORMAT_DD;
		break;

	case SAP_FORMAT2:
		sectorsize=SAP_SECTSIZE2;
		floppydisk->floppyNumberOfTrack=SAP_NTRACKS2;
		floppydisk->floppySectorPerTrack=SAP_NSECTS;
		floppydisk->floppyNumberOfSide=1;
		trackformat=ISOFORMAT_SD;
		break;
	default:
		floppycontext->hxc_printf(MSG_ERROR,"Unknow floppy format: %d !",floppyformat);
		sap_CloseArchive(sapid);
		return -1;
		break;

	}
		
	floppydisk->floppyBitRate=250000;
	floppydisk->floppyiftype=GENERIC_SHUGART_DD_FLOPPYMODE;
	floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);
	rpm=300; // normal rpm
	
			
	floppycontext->hxc_printf(MSG_INFO_1,"%d tracks, %d side(s), %d sectors/track,%d bytes/sector gap3:%d, interleave:%d,rpm:%d",floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack,sectorsize,gap3len,interleave,rpm);
			
	tracklen=(DEFAULT_DD_BITRATE/(rpm/60))/4;
	trackdata=(unsigned char*)malloc(sectorsize*floppydisk->floppySectorPerTrack);
			
	for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
	{
				
		floppydisk->tracks[j]=(CYLINDER*)malloc(sizeof(CYLINDER));
		currentcylinder=floppydisk->tracks[j];
		currentcylinder->number_of_side=floppydisk->floppyNumberOfSide;
		currentcylinder->sides=(SIDE**)malloc(sizeof(SIDE*)*currentcylinder->number_of_side);
		memset(currentcylinder->sides,0,sizeof(SIDE*)*currentcylinder->number_of_side);
				
		for(i=0;i<floppydisk->floppyNumberOfSide;i++)
		{
					
			currentcylinder->floppyRPM=rpm;
					
			currentcylinder->sides[i]=malloc(sizeof(SIDE));
			memset(currentcylinder->sides[i],0,sizeof(SIDE));
			currentside=currentcylinder->sides[i];
				
			currentside->number_of_sector=floppydisk->floppySectorPerTrack;
			currentside->tracklen=tracklen;
					
			currentside->databuffer=malloc(currentside->tracklen);
			memset(currentside->databuffer,0,currentside->tracklen);
			
			currentside->flakybitsbuffer=0;
					
			currentside->timingbuffer=0;
			currentside->bitrate=DEFAULT_DD_BITRATE;
			if(trackformat==ISOFORMAT_DD)
				currentside->track_encoding=ISOIBM_MFM_ENCODING;
			else
				currentside->track_encoding=ISOIBM_FM_ENCODING;

			currentside->indexbuffer=malloc(currentside->tracklen);
			memset(currentside->indexbuffer,0,currentside->tracklen);
					
			sap_ReadSectorEx(sapid,j,1,SAP_NSECTS, trackdata);
			
			BuildISOTrack(floppycontext,trackformat,currentside->number_of_sector,1,sectorsize,j,i,gap3len,trackdata,currentside->databuffer,&currentside->tracklen,interleave,0,NULL);

			currentside->tracklen=currentside->tracklen*8;

			fillindex(0,currentside,2500,TRUE,0);
		
		}
	}

	free(trackdata);
		
	sap_CloseArchive(sapid);

	floppycontext->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");
		
	return LOADER_NOERROR;
	
}
