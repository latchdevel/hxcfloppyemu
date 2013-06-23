/*
//
// Copyright (C) 2006-2013 Jean-Fran�ois DEL NERO
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
//----------------------------------------------------- http://hxc2001.com ------//
///////////////////////////////////////////////////////////////////////////////////
// File : floppy_utils.c
// Contains: HxC Floppy Emulator Endurance / Stress test software.
//
// Written by: DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <math.h>
#include <bios.h>
#include <dos.h>
#include <conio.h>
#include <i86.h>

#include "upd765.h"
#include "dma.h"
#include "daccess.h"
#include "testhxcfe.h"

#include "print_output.h"
#include "floppyio.h"

//#define	DBGMODE	1

int	errorcnt,readerror,writeerror;

unsigned char tracksectors[32][1024];

extern unsigned char *bufrd;
extern unsigned char *bufwr;

unsigned int sector_size_table_mfm_500[]={256,1024,256,512,256,512,256,1024,512,256,1024,256,256,512,512,512,1024,1024,0};
unsigned int sector_size_table_fm_500[]={256,1024,128,256,128,512,128,128,128,256,512,512,256,1024,128,0};

unsigned int sector_size_table_mfm_300[]={256,1024,256,512,512,256,256,512,512,256,1024,1024,0};
unsigned int sector_size_table_fm_300[]={256,1024,128,256,128,128,512,512,128,128,0};

unsigned int sector_size_table_mfm_250[]={256,1024,256,512,512,256,256,512,256,1024,0};
unsigned int sector_size_table_fm_250[]={256,1024,128,256,128,128,128,512,128,0};

typedef struct _track_test{
	unsigned int sectorsize;
	unsigned int nbsector;
	unsigned int gap3min;
	unsigned int gap3max;
}track_test;

track_test dd_track_test[]=
{
	{256,16,18,30},
	{512,10,26,36},
	{1024,4,26,74},
	{2048,2,36,94},
	{4096,1,36,34},
	{0,0,0,0}
};

int	randomaccess(unsigned long nbsect)
{
	unsigned long i;
	int	track,head,sector;
	unsigned char sectorbuf[512];
	unsigned short status;

	errorcnt=0;

	hxc_printf(0,"Random Access test...\n");
	for(i=0;i<nbsect;i++)
	{
		track=rand()%80;
		head=rand()%2;
		sector=rand()%19;
		if(!sector)	sector=1;

		hxc_printf(0,"%u : ",i);
		hxc_printf(0,"Track:%d Head:%d Sector:%d (E:%d)\n",track,head,sector,errorcnt);
		status=chs_biosdisk(_DISK_READ,0, head, track,sector, 1, &sectorbuf);
		if(status)
		{
			hxc_printf(0,"------------Failed!!!!---------\n");
			status = chs_biosdisk(_DISK_RESET,0, 0, 255,0, 1, &sector);
			errorcnt++;
//			for(;;);
		}
	}

	hxc_printf(0,"Ok!!!!\n");
	return	0;
}

int forceaccess()
{
	int i,j;
	int track;
	int side;
	unsigned char c;

	fdc_specify(2);

	hxc_printf(0,"+ : inc track, - dec track, s: change side, 0-9, r, i\n");

	track = 0;
	side = 0;

	do
	{
		hxc_printf(0,"Track: %d, Side: %d\n",track,side);
		trackseek(0,track,side);
		c=getch();
		switch(c)
		{
			case '+':
				if(track<256) track++;
			break;
			case '-':
				if(track) track--;
			break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				track = (c - '0') * 10;
			break;
			case 'r':
				for(i=0;i<100;i++)
				{
					track = rand()%80;
					side = rand()%1;
					hxc_printf(0,"Track: %d, Side: %d\n",track,side);
					trackseek(0,track,side);
				}
			break;
			case 'i':
				for(;;)
				{
					for(j=0;j<2;j++)
					{
						for(i=0;i<80;i++)
						{
							hxc_printf(0,"Track: %d, Side: %d\n",i,j);
							trackseek(0,i,j);
						}
					}
				}
			break;
			case 's':
				if(side)
					side = 0;
				else
					side = 1;
			break;
		}

	}while(1);

}

int	testdrive(int drive,unsigned int * secttable, int trackformat,unsigned int * secttableT0S0, int trackformatT0S0,unsigned int * secttableT0S1, int trackformatT0S1,int bitrate,int readonly)
{
	unsigned int i,j,ret,c;
	unsigned char track,head,precomp;

	unsigned int * sectortable;
	unsigned char deleted;
	int	density,fail,failcnt,okcnt,gap3;

	deleted = 0x00;
	precomp = 0;
	fail    = 0;
	failcnt = 0;
	okcnt   = 0;
	gap3 = 30;

	for(;;)
	{

		track=(rand()%80);
		head=rand()%2;

		if(!track) track =1;

		if(track)
		{
			sectortable = secttable;
			density = trackformat;
		}
		else
		{
			if(head)
			{
				sectortable = secttableT0S1;
				density = trackformatT0S1;
			}
			else
			{
				sectortable = secttableT0S0;
				density = trackformatT0S0;
			}
		}

		hxc_printf(0,"Track:%d, Side:%d, density:%d, Precomp: %d, deleted : 0x%.2x Rate:%d\n",track,head,density,precomp&7,deleted,bitrate);

		trackseek(drive,track,head);

		i=0;
		while(sectortable[i])
		{
			for(j=0;j<sectortable[i];j++)
			{
				tracksectors[i][j]=rand();
			}
			//tracksectors[i][0]=0xAA;
			//tracksectors[i][sectortable[i]-1]=0x55;
			i++;
		}

		if(!readonly)
		{
			i=0;
			while(sectortable[i])
			{
				memcpy(bufwr,&tracksectors[i],sectortable[i]);
				ret = write_sector(deleted,1+i,drive,head,track,1,1,sectortable[i],density,precomp,bitrate,gap3);
				fd_result(1);
				if(ret)
				{
					writeerror++;
				}
				i++;
			}
		}

		i=0;
		while(sectortable[i])
		{

			c=0;
			do
			{
				ret = read_sector(deleted,1+i,drive,head,track,1,1,sectortable[i],density,bitrate,gap3);
				fd_result(1);
				if(ret)
				{
					readerror++;
					hxc_printf(0,"Read Error Track %d Side %d Sector %d Deleted:%d Size :%d Retry...\n",track,head,1+i,deleted,sectortable[i]);
				}
				c++;
			}while(ret && c<5);

			if(!readonly)
			{
				if(memcmp(bufrd,&tracksectors[i],sectortable[i]))
				{
					hxc_printf(0,"-!-!-!-!-!-!-Write diff-!-!-!-!-!-!- : Sector %d Size:%d\n",i+1,sectortable[i]);
					fail = 1;
					//for(;;);
				}
			}

			i++;
		}

		if(!fail)
		{
			okcnt++;
			if(readonly)
				hxc_printf(0,"---------Read  ok--------- ok: %d fail: %d rderr %d wrerr %d\n",okcnt,failcnt,readerror,writeerror);
			else
				hxc_printf(0,"---------Write ok--------- ok: %d fail: %d rderr %d wrerr %d\n",okcnt,failcnt,readerror,writeerror);
		}
		else
		{
			failcnt++;
			hxc_printf(0,"---!--->>Write Failed<<---!--- ok: %d fail: %d rderr %d\n",okcnt,failcnt,readerror,writeerror);

			trackseek(0,1,0);
			calibratedrive(0);
			fail = 0;
		}

		if(!readonly)
			deleted=~deleted;

		precomp++;
	}
}

void format_write_read(int drive,int density,int bitrate)
{
	int track,ret,i;
	int side,nbsector,sector;
	int sectorsize,precomp,failcnt;
	unsigned char formatvalue,gap3,fvalue;
	int trackindex;
	unsigned int cycle;

	precomp = 0;
	failcnt = 0;
	gap3 = 30;
	formatvalue = 0x00;
	sectorsize = 512;

	cycle = 0;
	trackindex = 0;

	if(density)
	{
		switch(bitrate)
		{
			case 250:
				nbsector = 10;
			break;
			case 300:
				nbsector = 12;
			break;
			case 500:
				nbsector = 20;
			break;
		}
	}
	else
	{
		switch(bitrate)
		{
			case 250:
				nbsector = 5;
			break;
			case 300:
				nbsector = 6;
			break;
			case 500:
				nbsector = 10;
			break;
		}
	}

	do{

		if(!dd_track_test[trackindex].sectorsize)
			trackindex = 0;

		sectorsize = dd_track_test[trackindex].sectorsize;
		nbsector = dd_track_test[trackindex].nbsector;
		gap3 = dd_track_test[trackindex].gap3min;

		fvalue = formatvalue;
		for(track=0;track<80;track++)
		{
			trackseek(drive,track,side);
			for(side=0;side<=1;side++)
			{
				hxc_printf(0,"Format:%dx%d, T:%d, S:%d, Dens:%d, WComp:%d, Rate:%d, Gap:%d\n",sectorsize,nbsector,track,side,density,precomp&7,bitrate,gap3);
				format_track(drive,density,track,side,nbsector,sectorsize,1,formatvalue,precomp,bitrate,gap3);
				fd_result(1);
				formatvalue++;

				gap3++;
				if(gap3>dd_track_test[trackindex].gap3max)
					gap3 = dd_track_test[trackindex].gap3min;
			}
		}

		hxc_printf(0,"-----------------------------------------------------\n");
		hxc_printf(0,"---- Test %d - fail: %d rderr %d wrerr %d ----\n",cycle,failcnt,readerror,writeerror);
		hxc_printf(0,"-----------------------------------------------------\n");

		gap3 = dd_track_test[trackindex].gap3min;
		formatvalue = fvalue;
		for(track=0;track<80;track++)
		{
			trackseek(drive,track,side);
			for(side=0;side<=1;side++)
			{
				hxc_printf(0,"Read:%dx%d, T:%d, S:%d, Dens:%d, WComp:%d, Rate:%d, Gap:%d\n",sectorsize,nbsector,track,side,density,precomp&7,bitrate,gap3);

				sector = 0;
				ret = read_sector(0,1+sector,drive,side,track,1,nbsector,sectorsize,density,bitrate,gap3);
				fd_result(1);
				if(ret)
				{
					readerror++;
					hxc_printf(0,"Read Error Track %d Side %d Sector %d Size :%d Retry...\n",track,side,1+sector,sectorsize);
				}

				memset(bufwr,formatvalue,nbsector*sectorsize);
				if(memcmp(bufwr,bufrd,nbsector*sectorsize))
				{
					hxc_printf(0,"Data Error Track %d Side %d Sector %d Size :%d\n",track,side,1+sector,sectorsize);
					readerror++;
					writeerror++;
					failcnt++;
				}

				formatvalue++;

				gap3++;
				if(gap3>dd_track_test[trackindex].gap3max)
					gap3 = dd_track_test[trackindex].gap3min;

			}
		}

		hxc_printf(0,"-----------------------------------------------------\n");
		hxc_printf(0,"---- Test %d - fail: %d rderr %d wrerr %d ----\n",cycle,failcnt,readerror,writeerror);
		hxc_printf(0,"-----------------------------------------------------\n");

		srand(fvalue);

		gap3 = dd_track_test[trackindex].gap3min;
		for(track=0;track<80;track++)
		{
			trackseek(drive,track,side);
			for(side=0;side<=1;side++)
			{
				hxc_printf(0,"Write:%dx%d, T:%d, S:%d, Dens:%d, WComp:%d, Rate:%d, Gap:%d\n",sectorsize,nbsector,track,side,density,precomp&7,bitrate,gap3);

				for(i=0;i<(nbsector*sectorsize);i++)
				{
					bufwr[i] = rand();
				}
				memset(bufrd,0,nbsector*sectorsize);

				sector = 0;
				ret = write_sector(0,1+sector,drive,side,track,1,nbsector,sectorsize,density,0,bitrate,gap3);
				fd_result(1);
				if(ret)
				{
					writeerror++;
					hxc_printf(0,"Write Error Track %d Side %d Sector %d Size :%d Retry...\n",track,side,1+sector,sectorsize);
				}

				gap3++;
				if(gap3>dd_track_test[trackindex].gap3max)
					gap3 = dd_track_test[trackindex].gap3min;

			}
		}

		hxc_printf(0,"-----------------------------------------------------\n");
		hxc_printf(0,"---- Test %d - fail: %d rderr %d wrerr %d ----\n",cycle,failcnt,readerror,writeerror);
		hxc_printf(0,"-----------------------------------------------------\n");

		srand(fvalue);
		gap3 = dd_track_test[trackindex].gap3min;

		for(track=0;track<80;track++)
		{
			trackseek(drive,track,side);
			for(side=0;side<=1;side++)
			{
				hxc_printf(0,"(W) Read:%dx%d, T:%d, S:%d, Dens:%d, WComp:%d, Rate:%d, Gap:%d\n",sectorsize,nbsector,track,side,density,precomp&7,bitrate,gap3);

				for(i=0;i<(nbsector*sectorsize);i++)
				{
					bufwr[i] = rand();
				}

				sector = 0;
				ret = read_sector(0,1+sector,drive,side,track,1,nbsector,sectorsize,density,bitrate,gap3);
				fd_result(1);
				if(ret)
				{
					readerror++;
					hxc_printf(0,"Read Error Track %d Side %d Sector %d Size :%d Retry...\n",track,side,1+sector,sectorsize);
				}

				if(memcmp(bufwr,bufrd,nbsector*sectorsize))
				{
					hxc_printf(0,"Write Data Error Track %d Side %d Sector %d Size :%d\n",track,side,1+sector,sectorsize);
					readerror++;
					writeerror++;
					failcnt++;
				}

				gap3++;
				if(gap3>dd_track_test[trackindex].gap3max)
					gap3 = dd_track_test[trackindex].gap3min;
			}
		}

		hxc_printf(0,"-----------------------------------------------------\n");
		hxc_printf(0,"---- Test %d - fail: %d rderr %d wrerr %d ----\n",cycle,failcnt,readerror,writeerror);
		hxc_printf(0,"-----------------------------------------------------\n");

		formatvalue++;
		trackindex++;

		cycle++;
	}while(1);
}
int	main(int argc, char* argv[])
{
	unsigned char  c;

	direct_access_cmd_sector dacs;

	init_outputtxt();

	writeerror=0;
	readerror=0;
	errorcnt=0;

	hxc_printf(0,"Test HxCFEDA\n");

	memset(&dacs,0,sizeof(direct_access_cmd_sector));
	sprintf(dacs.DAHEADERSIGNATURE,"HxCFEDA");

	init_floppyio();

	hxc_printf(0,"Choose:\n");
	hxc_printf(0,"1 - MFM500K.HFE\n");
	hxc_printf(0,"2 - MFM300K.HFE\n");
	hxc_printf(0,"3 - MFM250K.HFE\n");
	hxc_printf(0,"4 - FM500K.HFE\n");
	hxc_printf(0,"5 - FM300K.HFE\n");
	hxc_printf(0,"6 - FM250K.HFE\n");
	hxc_printf(0,"a - MFM500K.HFE (RO)\n");
	hxc_printf(0,"b - MFM300K.HFE (RO)\n");
	hxc_printf(0,"c - MFM250K.HFE (RO)\n");
	hxc_printf(0,"d - FM500K.HFE (RO)\n");
	hxc_printf(0,"e - FM300K.HFE (RO)\n");
	hxc_printf(0,"f - FM250K.HFE (RO)\n");
	hxc_printf(0,"T - Force Read\n");
	hxc_printf(0,"F - Format Write Read\n");

	do
	{
		c=getchar();
		switch(c)
		{
			case '1':
				testdrive(0,sector_size_table_mfm_500,1,sector_size_table_fm_500, 0,sector_size_table_fm_500,	0,500,0);
			break;
			case '2':
				testdrive(0,sector_size_table_mfm_300,1,sector_size_table_fm_300, 0,sector_size_table_fm_300,	0,300,0);
			break;
			case '3':
				testdrive(0,sector_size_table_mfm_250,1,sector_size_table_fm_250, 0,sector_size_table_fm_250, 0,250,0);
			break;
			case '4':
				testdrive(0,sector_size_table_fm_500,0,sector_size_table_mfm_500, 1,sector_size_table_mfm_500, 1,500,0);
			break;
			case '5':
				testdrive(0,sector_size_table_fm_300,0,sector_size_table_mfm_300, 1,sector_size_table_mfm_300, 1,300,0);
			break;
			case '6':
				testdrive(0,sector_size_table_fm_250,0,sector_size_table_mfm_250, 1,sector_size_table_mfm_250, 1,250,0);
			break;
			case 'a':
				testdrive(0,sector_size_table_mfm_500,1,sector_size_table_fm_500, 0,sector_size_table_fm_500, 0,500,1);
			break;
			case 'b':
				testdrive(0,sector_size_table_mfm_300,1,sector_size_table_fm_300, 0,sector_size_table_fm_300, 0,300,1);
			break;
			case 'c':
				testdrive(0,sector_size_table_mfm_250,1,sector_size_table_fm_250, 0,sector_size_table_fm_250, 0,250,1);
			break;
			case 'd':
				testdrive(0,sector_size_table_fm_500,0,sector_size_table_mfm_500, 1,sector_size_table_mfm_500, 1,500,1);
			break;
			case 'e':
				testdrive(0,sector_size_table_fm_300,0,sector_size_table_mfm_300, 1,sector_size_table_mfm_300, 1,300,1);
			break;
			case 'f':
				testdrive(0,sector_size_table_fm_250,0,sector_size_table_mfm_250, 1,sector_size_table_mfm_250, 1,250,1);
			break;
			case 'T':
				forceaccess();
			break;
			case 'F':
				format_write_read(0,1,250);
			break;

			default:
			break;
		}
	}while(1);

	return 0;
}


