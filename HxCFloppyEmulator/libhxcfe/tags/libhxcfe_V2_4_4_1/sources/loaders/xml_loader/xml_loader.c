/*
//
// Copyright (C) 2006-2014 Jean-Fran?ois DEL NERO
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
// File : xml_loader.c
// Contains: XML floppy image loader
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

#include "xml_loader.h"
#include "xml_writer.h"

#include "libhxcadaptor.h"

int XML_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int filesize;
	char firstline[512];
	FILE * f;

	floppycontext->hxc_printf(MSG_DEBUG,"XML_libIsValidDiskFile");

	if(hxc_checkfileext(imgfile,"xml"))
	{
		filesize=hxc_getfilesize(imgfile);
		if(filesize<0)
		{
			floppycontext->hxc_printf(MSG_ERROR,"XML_libIsValidDiskFile : Cannot open %s !",imgfile);
			return HXCFE_ACCESSERROR;
		}

		f=fopen(imgfile,"rb");
		if(f)
		{
			memset(firstline,0,sizeof(firstline));
			fgets(firstline,sizeof(firstline)-1,f);
			fclose(f);

			if(strstr(firstline,"<?xml version="))
			{
				floppycontext->hxc_printf(MSG_DEBUG,"XML_libIsValidDiskFile : XML file !");
				return HXCFE_VALIDFILE;
			}

			floppycontext->hxc_printf(MSG_DEBUG,"XML_libIsValidDiskFile : non XML file !");
			return HXCFE_BADFILE;

		}

		floppycontext->hxc_printf(MSG_ERROR,"XML_libIsValidDiskFile : Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}
	else
	{
		floppycontext->hxc_printf(MSG_DEBUG,"XML_libIsValidDiskFile : non XML file !");
		return HXCFE_BADFILE;
	}

	return HXCFE_BADPARAMETER;
}

int XML_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{
	FILE * f;
	unsigned int filesize;
	XmlFloppyBuilder* rfb;
	FLOPPY * fp;

	floppycontext->hxc_printf(MSG_DEBUG,"XML_libLoad_DiskFile %s",imgfile);

	f=hxc_fopen(imgfile,"rb");
	if(f==NULL)
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}

	fseek (f , 0 , SEEK_END);
	filesize=ftell(f);
	fseek (f , 0 , SEEK_SET);

	hxc_fclose(f);

	if(hxc_checkfileext(imgfile,"xml") && filesize!=0)
	{
		rfb = hxcfe_initXmlFloppy(floppycontext);
		if(rfb)
		{
			if(hxcfe_setXmlFloppyLayoutFile(rfb,imgfile) == HXCFE_NOERROR)
			{
				if(parameters)
				{
					fp = hxcfe_generateXmlFileFloppy(rfb,(char*)parameters);
				}
				else
				{
					fp = hxcfe_generateXmlFloppy(rfb,0,0);
				}

				if(fp)
				{
					memcpy(floppydisk,fp,sizeof(FLOPPY));
					free(fp);

					hxcfe_deinitXmlFloppy(rfb);

					floppycontext->hxc_printf(MSG_DEBUG,"XML_libLoad_DiskFile - disk generated !");

					return HXCFE_NOERROR;
				}
			}

			hxcfe_deinitXmlFloppy(rfb);
		}
	}


	return HXCFE_BADFILE;
}

int XML_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{
	static const char plug_id[]="GENERIC_XML";
	static const char plug_desc[]="Generic XML file Loader";
	static const char plug_ext[]="xml";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	XML_libIsValidDiskFile,
		(LOADDISKFILE)		XML_libLoad_DiskFile,
		(WRITEDISKFILE)		XML_libWrite_DiskFile,
		(GETPLUGININFOS)	XML_libGetPluginInfo
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
