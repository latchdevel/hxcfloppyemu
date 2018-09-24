/*
//
// Copyright (C) 2006-2018 Jean-Fran�ois DEL NERO
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
#include <time.h>

#include "types.h"

#include "version.h"

#include "internal_libhxcfe.h"
#include "tracks/track_generator.h"
#include "libhxcfe.h"

#include "imd_format.h"

#include "libhxcadaptor.h"

extern unsigned char size_to_code(uint32_t size);

int IMD_libWrite_DiskFile(HXCFE_IMGLDR* imgldr_ctx,HXCFE_FLOPPY * floppy,char * filename)
{
	int32_t i,j,k,l,nbsector;
	FILE * imdfile;
	char * log_str;
	char   tmp_str[256];
	char rec_mode;
	unsigned char sector_type;

	unsigned char sector_numbering_map[256];
	unsigned char cylinder_numbering_map[256];
	unsigned char side_numbering_map[256];

	int32_t track_cnt,bitrate;

	imd_trackheader imd_th;

	struct tm * ts;
	time_t currenttime;

	HXCFE_SECTORACCESS* ss;
	HXCFE_SECTCFG** sca;

//	struct DateTime reptime;


	imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,"Write IMD file %s...",filename);

	log_str=0;
	imdfile=hxc_fopen(filename,"wb");
	if(imdfile)
	{

		currenttime=time (NULL);
		ts=localtime(&currenttime);

		fprintf(imdfile,"IMD 1.17: %.2d/%.2d/%.4d %.2d:%.2d:%.2d\r\n",ts->tm_mday,ts->tm_mon,ts->tm_year+1900,ts->tm_hour,ts->tm_min,ts->tm_sec);
		fprintf(imdfile,"File generated by the HxC Floppy Emulator software v%s\r\n",STR_FILE_VERSION2);
		fprintf(imdfile,"%c",0x1A);

		memset(sector_numbering_map,0,0x100);
		memset(cylinder_numbering_map,0,0x100);
		memset(side_numbering_map,0,0x100);

		track_cnt=0;

		ss=hxcfe_initSectorAccess(imgldr_ctx->hxcfe,floppy);
		if(ss)
		{

			for(j=0;j<(int)floppy->floppyNumberOfTrack;j++)
			{
				for(i=0;i<(int)floppy->floppyNumberOfSide;i++)
				{
					hxcfe_imgCallProgressCallback(imgldr_ctx,(j<<1) + (i&1),(2*floppy->floppyNumberOfTrack) );

					sprintf(tmp_str,"track:%.2d:%d file offset:0x%.6x, sectors: ",j,i,(unsigned int)ftell(imdfile));

					log_str=0;
					log_str=realloc(log_str,strlen(tmp_str)+1);
					memset(log_str,0,strlen(tmp_str)+1);
					strcat(log_str,tmp_str);

					rec_mode=2;

					sca = hxcfe_getAllTrackSectors(ss,j,i,ISOIBM_MFM_ENCODING,&nbsector);
					if(!sca)
					{
						sca = hxcfe_getAllTrackSectors(ss,j,i,ISOIBM_FM_ENCODING,&nbsector);
						rec_mode=1;
						if(!nbsector)
						{
							rec_mode=3;
							sca = hxcfe_getAllTrackSectors(ss,j,i,MEMBRAIN_MFM_ENCODING,&nbsector);
						}
					}

					memset(&imd_th,0,sizeof(imd_trackheader));

					imd_th.physical_head=i;

					l=0;
					while((l<nbsector) && sca[l]->head == i )
					{
						l++;
					}
					if(l!=nbsector)
					{
						imd_th.physical_head=imd_th.physical_head | 0x40;
					}


					l=0;
					while((l<nbsector) && sca[l]->cylinder == j)
					{
						l++;
					}
					if(l!=nbsector)
					{
						imd_th.physical_head=imd_th.physical_head | 0x80;
					}

					imd_th.physical_cylinder=j;
					imd_th.number_of_sector=nbsector;

					imd_th.track_mode_code=rec_mode;

					bitrate = floppy->tracks[j]->sides[i]->bitrate;
					if(floppy->tracks[j]->sides[i]->timingbuffer)
					{
						bitrate = floppy->tracks[j]->sides[i]->timingbuffer[2];
					}

					if(bitrate < 550000 && bitrate > 450000)
						bitrate = 500000;
					if(bitrate < 350000 && bitrate > 280000)
						bitrate = 300000;
					if(bitrate < 280000 && bitrate > 230000)
						bitrate = 250000;

					switch(bitrate)
					{
						case 250000:
							imd_th.track_mode_code=2;
							break;
						case 300000:
							imd_th.track_mode_code=1;
							break;
						case 500000:
							imd_th.track_mode_code=0;
							break;
						default:
							imd_th.track_mode_code=2;
							break;
					}

					if(rec_mode==2)
					{
						imd_th.track_mode_code=imd_th.track_mode_code+3;
					}

					if(rec_mode==3)
					{
						imd_th.track_mode_code=imd_th.track_mode_code+0x80;
					}

					if(nbsector)
					{
						imd_th.sector_size_code=size_to_code(sca[0]->sectorsize);
					}

					fwrite(&imd_th,sizeof(imd_trackheader),1,imdfile);

					for(k=0;k<nbsector;k++)
					{
						sector_numbering_map[k] = sca[k]->sector;
						cylinder_numbering_map[k] = sca[k]->cylinder;
						side_numbering_map[k]= sca[k]->head;
					}

					fwrite(sector_numbering_map,imd_th.number_of_sector,1,imdfile);
					if(imd_th.physical_head & 0x80)fwrite(cylinder_numbering_map,imd_th.number_of_sector,1,imdfile);
					if(imd_th.physical_head & 0x40)fwrite(side_numbering_map,imd_th.number_of_sector,1,imdfile);

					if(nbsector)
					{

						k=0;
						do
						{
							if(sca[k]->input_data)
							{
								l=0;
								while((l<(int)sca[k]->sectorsize) && sca[k]->input_data[l]==sca[k]->input_data[0])
								{
									l++;
								}

								sector_type = 0;
								if(l!=(int)sca[k]->sectorsize)
								{
									// Non compressed
									if(!sca[k]->use_alternate_data_crc)
									{
										// CRC ok
										switch(sca[k]->alternate_datamark)
										{
											case 0xFB:
												sector_type = 1;
											break;
											case 0xF8: // Deleted datamark
												sector_type = 3;
											break;
											default:
												sector_type = 1;
											break;
										}
									}
									else
									{
										// CRC Error
										switch(sca[k]->alternate_datamark)
										{
											case 0xFB:
												sector_type = 5;
											break;
											case 0xF8: // Deleted datamark
												sector_type = 7;
											break;
											default:
												sector_type = 5;
											break;
										}
									}

									fwrite(&sector_type,1,1,imdfile);
									fwrite(sca[k]->input_data,sca[k]->sectorsize,1,imdfile);
								}
								else
								{
									// Compressed
									if(!sca[k]->use_alternate_data_crc)
									{
										// CRC ok
										switch(sca[k]->alternate_datamark)
										{
											case 0xFB:
												sector_type = 2;
											break;
											case 0xF8: // Deleted datamark
												sector_type = 4;
											break;
											default:
												sector_type = 2;
											break;
										}
									}
									else
									{
										// CRC Error
										switch(sca[k]->alternate_datamark)
										{
											case 0xFB:
												sector_type = 6;
											break;
											case 0xF8: // Deleted datamark
												sector_type = 8;
											break;
											default:
												sector_type = 6;
											break;
										}
									}
									fwrite(&sector_type,1,1,imdfile);
									fwrite(sca[k]->input_data,1,1,imdfile);
								}
							}
							else
							{
								// Compressed
								if(!sca[k]->use_alternate_data_crc)
								{
									// CRC ok
									switch(sca[k]->alternate_datamark)
									{
										case 0xFB:
											sector_type = 2;
										break;
										case 0xF8: // Deleted datamark
											sector_type = 4;
										break;
										default:
											sector_type = 2;
										break;
									}
								}
								else
								{
									// CRC Error
									switch(sca[k]->alternate_datamark)
									{
										case 0xFB:
											sector_type = 6;
										break;
										case 0xF8: // Deleted datamark
											sector_type = 8;
										break;
										default:
											sector_type = 6;
										break;
									}
								}
								fwrite(&sector_type,1,1,imdfile);
								fwrite(&sca[k]->fill_byte,1,1,imdfile);
							}

							sprintf(tmp_str,"%d ",sca[k]->sector);
							log_str=realloc(log_str,strlen(log_str)+strlen(tmp_str)+1);
							strcat(log_str,tmp_str);
							k++;

						}while(k<nbsector);

						k=0;
						do
						{
							hxcfe_freeSectorConfig( ss , sca[k] );
							k++;
						}while(k<nbsector);

						if(sca)
							free(sca);

						log_str = realloc(log_str,strlen(log_str)+strlen(tmp_str)+1);
						strcat(log_str,tmp_str);

					}

					track_cnt++;

					imgldr_ctx->hxcfe->hxc_printf(MSG_INFO_1,log_str);
					free(log_str);

				}

			}

			hxcfe_deinitSectorAccess(ss);
		}

		hxc_fclose(imdfile);
	}

	return 0;
}
