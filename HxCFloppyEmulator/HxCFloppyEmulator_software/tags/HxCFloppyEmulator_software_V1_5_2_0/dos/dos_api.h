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

typedef int (*THREADFUNCTION) (void* floppyemulator,void* hwemulator);

typedef struct threadinit_
{
	THREADFUNCTION thread;
	HXCFLOPPYEMULATOR * hxcfloppyemulatorcontext;
	void * hwcontext;
}threadinit;

int getlistoffile(unsigned char * directorypath,unsigned char *** filelist);
char * getcurrentdirectory(char *currentdirectory,int buffersize);
int loaddiskplugins(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * newfloppy,char *pluginpath);
int hxc_createthread(HXCFLOPPYEMULATOR* floppycontext,void * hwcontext,THREADFUNCTION thread,int priority);
//int createthread(HXCFLOPPYEMULATOR* floppycontext,THREADFUNCTION thread,int priority);
unsigned long hxc_createevent(HXCFLOPPYEMULATOR* floppycontext,unsigned char id);

int hxc_waitevent(HXCFLOPPYEMULATOR* floppycontext,int id,int timeout);
int hxc_setevent(HXCFLOPPYEMULATOR* floppycontext,unsigned char id);
unsigned long hxc_create_ftdi_event(HXCFLOPPYEMULATOR* floppycontext,unsigned char id);
void hxc_pause(int ms);

typedef struct filefoundinfo_
{
	int isdirectory;
	char * filename;
	char * fullpath;
	int size;
}filefoundinfo;

long find_close(long handleff);
long find_next_file(long handleff,char *folder,char *file,filefoundinfo* fileinfo);
long find_first_file(char *folder,char *file,filefoundinfo* fileinfo);




