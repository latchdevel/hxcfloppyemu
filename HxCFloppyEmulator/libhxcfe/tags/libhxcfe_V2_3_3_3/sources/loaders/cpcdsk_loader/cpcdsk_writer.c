/*
//
// Copyright (C) 2006 - 2013 Jean-Fran?ois DEL NERO
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libhxcfe.h"

#include "cpcdsk_format.h"
#include "cpcdsk_loader.h"

#include "tracks/sector_extractor.h"

#include "libhxcadaptor.h"

unsigned char  size_to_code(unsigned long size)
{

	switch(size)
	{
		case 128:
			return 0;
		break;
		case 256:
			return 1;
		break;
		case 512:
			return 2;
		break;
		case 1024:
			return 3;
		break;
		case 2048:
			return 4;
		break;
		case 4096:
			return 5;
		break;
		case 8192:
			return 6;
		break;
		case 16384:
			return 7;
		break;
		default:
			return 0;
		break;
	}
}

int CPCDSK_libWrite_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppy,char * filename)
{	
	int i,j,k,nbsector;
	FILE * cpcdskfile;
	char * log_str;
	char   tmp_str[256];
	char   disk_info_block[256];
	char rec_mode;
	int sectorsize;
	int track_cnt;
	int sectorlistoffset,trackinfooffset;
	cpcdsk_fileheader * cpcdsk_fh;
	cpcdsk_trackheader cpcdsk_th;
	cpcdsk_sector cpcdsk_s;

	SECTORSEARCH* ss;
	SECTORCONFIG** sca;


	floppycontext->hxc_printf(MSG_INFO_1,"Write CPCDSK file %s...",filename);

	log_str=0;
	cpcdskfile=hxc_fopen(filename,"wb");
	if(cpcdskfile)
	{
		memset(disk_info_block,0,0x100);
		cpcdsk_fh=(cpcdsk_fileheader *)&disk_info_block;
		sprintf((char*)&cpcdsk_fh->headertag,"EXTENDED CPC DSK File\r\nDisk-Info\r\n");
		sprintf((char*)&cpcdsk_fh->creatorname,"HxCFloppyEmu\r\n");
		fwrite(&disk_info_block,0x100,1,cpcdskfile);
		track_cnt=0;

		ss=hxcfe_initSectorSearch(floppycontext,floppy);
		
		if(ss)
		{
			for(j=0;j<(int)floppy->floppyNumberOfTrack;j++)
			{
				for(i=0;i<(int)floppy->floppyNumberOfSide;i++)
				{
					sprintf(tmp_str,"track:%.2d:%d file offset:0x%.6x, sectors: ",j,i,(unsigned int)ftell(cpcdskfile));

					log_str=0;
					log_str=realloc(log_str,strlen(tmp_str)+1);
					memset(log_str,0,strlen(tmp_str)+1);
					strcat(log_str,tmp_str);

					rec_mode=0;
					rec_mode=2;
					sca = hxcfe_getAllTrackSectors(ss,j,i,ISOIBM_MFM_ENCODING,&nbsector);
					if(!sca)
					{
						sca = hxcfe_getAllTrackSectors(ss,j,i,ISOIBM_FM_ENCODING,&nbsector);
						rec_mode=1;
						if(!nbsector)
						{
							rec_mode=0;
							sca = hxcfe_getAllTrackSectors(ss,j,i,AMIGA_MFM_ENCODING,&nbsector);
						}
					}

					memset(&cpcdsk_th,0,sizeof(cpcdsk_trackheader));
					sprintf(cpcdsk_th.headertag,"Track-Info\r\n");
					cpcdsk_th.side_number=i;
					cpcdsk_th.track_number=j;
					cpcdsk_th.gap3_length=78;
					cpcdsk_th.filler_byte=0xE5;
					cpcdsk_th.number_of_sector = nbsector;
					cpcdsk_th.rec_mode=rec_mode;

					switch(floppy->tracks[j]->sides[i]->bitrate)
					{
						case 250000:
							cpcdsk_th.datarate=1;
							break;
						case 500000:
							cpcdsk_th.datarate=2;
							break;
						case 1000000:
							cpcdsk_th.datarate=3;
							break;
						default:
							cpcdsk_th.datarate=0;
							break;

					}


					if(nbsector)
					{
						cpcdsk_th.sector_size_code=size_to_code(sca[0]->sectorsize);
					}

					trackinfooffset=ftell(cpcdskfile);
					fwrite(&cpcdsk_th,sizeof(cpcdsk_trackheader),1,cpcdskfile);
					sectorlistoffset=ftell(cpcdskfile);

					if(nbsector)
					{
						if(cpcdsk_fh->number_of_sides<(i+1))cpcdsk_fh->number_of_sides=i+1;
						if(cpcdsk_fh->number_of_tracks<(j+1))cpcdsk_fh->number_of_tracks=j+1;


						memset(&cpcdsk_s,0,sizeof(cpcdsk_sector));
						for(k=0;k<nbsector;k++) 
						{
							fwrite(&cpcdsk_s,sizeof(cpcdsk_sector),1,cpcdskfile);
						}
						memset(tmp_str,0,0x100);
						fwrite(&tmp_str,0x100-(((sizeof(cpcdsk_sector)*nbsector)+sizeof(cpcdsk_trackheader))%0x100),1,cpcdskfile);

						sectorsize=sca[0]->sectorsize;

						k=0;
						do
						{
							if(sca[k]->sectorsize!=(unsigned int)sectorsize)
							{
								sectorsize=-1;
							}


							cpcdsk_s.sector_id = sca[k]->sector;
							cpcdsk_s.side = sca[k]->head;
							cpcdsk_s.track = sca[k]->cylinder;
							cpcdsk_s.sector_size_code = size_to_code(sca[k]->sectorsize);
							cpcdsk_s.data_lenght = sca[k]->sectorsize;								
							fseek(cpcdskfile,sectorlistoffset+(k*sizeof(cpcdsk_sector)),SEEK_SET);
							fwrite(&cpcdsk_s,sizeof(cpcdsk_sector),1,cpcdskfile);

							fseek(cpcdskfile,0,SEEK_END);
							fwrite(sca[k]->input_data,sca[k]->sectorsize,1,cpcdskfile);

							sprintf(tmp_str,"%d ",sca[k]->sector);
							log_str=realloc(log_str,strlen(log_str)+strlen(tmp_str)+1);
							strcat(log_str,tmp_str);
							k++;

						}while(k<nbsector);

						k=0;
						do
						{
							free(sca[k]->input_data);
							free(sca[k]);
							k++;
						}while(k<nbsector);

						if(sectorsize!=-1)
						{
							sprintf(tmp_str,",%dB/s",sectorsize);
						}

						log_str=realloc(log_str,strlen(log_str)+strlen(tmp_str)+1);
						strcat(log_str,tmp_str);

					}

					disk_info_block[sizeof(cpcdsk_fileheader)+track_cnt]=(ftell(cpcdskfile)-trackinfooffset)/256;
					track_cnt++;

					floppycontext->hxc_printf(MSG_INFO_1,log_str);
					free(log_str);

				}
			}

			hxcfe_deinitSectorSearch(ss);

		}

		fseek(cpcdskfile,0,SEEK_SET);
		fwrite(&disk_info_block,0x100,1,cpcdskfile);

		hxc_fclose(cpcdskfile);
	}

	return 0;
}
