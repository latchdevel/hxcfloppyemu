#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include "stdint.h"
#include <windows.h>
#else
#include <stdint.h>
#include "bmp_file.h"
#endif

#include "../../packer/pack.h"
#include "../../packer/lzw.h"
#include "../../packer/rle.h"

typedef struct {
	uint32_t xres;
	uint32_t yres;
	uint32_t size;
	uint32_t csize;
	uint8_t type;
	uint8_t pack;
} bmpinfo;

uint8_t * extractbmpdata(char *bmpfile,bmpinfo *info)
{
	FILE * file;
	int p,m,i,j,o,k;
	int32_t s;
	uint32_t filesize,adj_filesize;
	uint8_t * dbuffer;
	uint8_t vc;

	BITMAPFILEHEADER bmph;
	BITMAPINFOHEADER bmih;
	uint8_t pallette[256*8];

	dbuffer = NULL;

	memset(&bmph,0,sizeof(BITMAPFILEHEADER));
	memset(&bmih,0,sizeof(BITMAPINFOHEADER));

	s = 0;

	file=fopen(bmpfile,"rb");
	if( file )
	{
		//Get the file size
		fseek(file,0,SEEK_END);
		filesize = ftell(file);
		fseek(file,0,SEEK_SET);

		// read header
		if(info->type!=0xff)
		{
			if(fread(&bmph,sizeof(bmph),1,file) != 1)
				goto error;

			if(fread(&bmih,sizeof(bmih),1,file) != 1)
				goto error;

			info->xres=bmih.biWidth;
			info->yres=bmih.biHeight;

			if(info->type==9)
			{
				if(fread(&pallette,256*4,1,file) != 1)
					goto error;
			}

			s=bmih.biWidth;
			do
			{
				s=s-4;
			}while(s>=4);

			if(s!=0)
				s=4-s;
		}
		//info->type=bmih.biBitCount;

		adj_filesize=((filesize-bmph.bfOffBits)-(s*bmih.biHeight));

		if(info->type==0xff)
			adj_filesize = filesize;

		if(info->type==9)
			adj_filesize += (3*256);

		if(info->type==1)
			info->size = adj_filesize/8;
		else
			info->size = adj_filesize;

		//mem data
		dbuffer = (uint8_t *) malloc(adj_filesize+100);
		if(!dbuffer)
			goto error;

		memset(dbuffer,0,adj_filesize+100);

		p=0;
		j=0;
		if(info->type==9)
		{
			for(i=0;i<256;i++)
			{
				dbuffer[(i*3)]=pallette[(i*4)];
				dbuffer[(i*3)+1]=pallette[(i*4)+1];
				dbuffer[(i*3)+2]=pallette[(i*4)+2];
			}

			p=3*256;
			j=p;
		}

		fseek(file,bmph.bfOffBits,SEEK_SET);

		m=0;

		k=0;
		vc=0;

		for(i=0;i<(int)(adj_filesize-j);i++)
		{

			if(info->type==1)
			{
				if(fgetc(file)) vc=vc|(0x80>>k);
				k++;
				if(k>=8)
				{
					dbuffer[p]=vc;
					p++;
					k=0;
					vc=0;
				}

			}
			else
			{
				dbuffer[p]=fgetc(file);
				p++;
			}

			//sup du padding bmp
			if(info->type!=0xff)
			{
				m++;
				if(m>=bmih.biWidth)
				{
					for(o=0;o<s;o++)fgetc(file);
					m=0;
				}
			}
		}

		fclose(file);
		return dbuffer;
	}
	return NULL;

error:
	if(file)
		fclose(file);

	if(dbuffer)
		free(dbuffer);

	return NULL;
}

char  buildincludefile(char *includefile,bmpinfo *info,unsigned char * dbuffer)
{
	int l;
	unsigned int i;
	FILE * file2;

	char temp[128];
	char temp2[128];

	sprintf(temp,"%s",includefile);
	for(i=0;i<strlen(temp);i++)
	{
		if(temp[i]=='.') temp[i]='_';
	}

	if(info->type!=0xff)
		sprintf(temp2,"data_bmp_%s.h",temp);
	else
		sprintf(temp2,"data_%s.h",temp);

	printf("Create %s :",temp2);

	file2 = fopen(temp2,"w");
	if(!file2)
		return -1;

	if(info->type!=0xff)
	{
		fprintf(file2,"//////////////////////////////\n//\n//\n// Created by Binary2Header V0.6\n// (c) HxC2001\n// (c) PowerOfAsm\n//\n");
		fprintf(file2,"// File: %s  Size: %d  (%d) x:%d y:%d\n//\n//\n",includefile,info->size,info->csize,info->xres,info->yres);
		fprintf(file2,"\n\n");
		fprintf(file2,"#ifndef BMAPTYPEDEF\n#define BMAPTYPEDEF\n\n");
		fprintf(file2,"typedef  struct _bmaptype\n{\n   int type;\n   int Xsize;\n   int Ysize;\n   int size;\n   int csize;\n   unsigned char * data;\n   unsigned char * unpacked_data;\n}bmaptype;\n\n");
		fprintf(file2,"#endif\n");
		fprintf(file2,"\n\n");
		fprintf(file2,"unsigned char data_bmp%s[]={\n",temp);
	}
	else
	{
		fprintf(file2,"//////////////////////////////\n//\n//\n// Created by Binary2Header V0.6\n// (c) HxC2001\n// (c) PowerOfAsm\n//\n");
		fprintf(file2,"// File: %s  Size: %d  (%d) \n//\n//\n",includefile,info->size,info->csize);
		fprintf(file2,"\n\n");
		fprintf(file2,"#ifndef DATATYPEDEF\n#define DATATYPEDEF\n\n");
		fprintf(file2,"typedef  struct _datatype\n{\n   int type;\n   int size;\n   int csize;\n   unsigned char * data;\n   unsigned char * unpacked_data;\n}datatype;\n\n");
		fprintf(file2,"#endif\n");
		fprintf(file2,"\n\n");
		fprintf(file2,"unsigned char data__%s[]={\n",temp);

	}

	l=0;
	for(i=0;i<info->csize;i++)
	{
		{
			fprintf(file2,"0x%.2x",dbuffer[i]);
			if((i+1)<info->csize)fprintf(file2,",");
		}

		l++;
		if(l>=10)
		{
			l=0;
			fprintf(file2,"\n");
		}
	}

	fprintf(file2,"};\n");

	if(info->type!=0xff)
		fprintf(file2,"\n\nstatic bmaptype bitmap_%s[]=\n{\n\t{ %d, %d, %d, %d, %d, data_bmp%s, 0 }\n};\n",temp,info->type,info->xres,info->yres,info->size,info->csize,temp);
	else
		fprintf(file2,"\n\nstatic datatype data_%s[]=\n{\n\t{ %d, %d, %d, data__%s, 0 }\n};\n",temp,info->type,info->size,info->csize,temp);

	fclose(file2);
	return 0;
}


int mi_pack(unsigned char * bufferin, int sizein,unsigned char * bufferout, int * sizeout)
{
	unsigned char* buffer;
	unsigned char* buffer2;
	int  newsize;
	int  newsize_lzw;
	int  newsize_rle;
	int mode;

	buffer =  (unsigned char*)malloc(sizein * 10);
	buffer2 = (unsigned char*)malloc(sizein * 10);

	mode = -1;

	if( buffer && buffer2)
	{
		memset(buffer, 0, sizein * 10);
		memset(buffer2, 0, sizein * 10);

		newsize_lzw = sizein * 10;

		lzw_compress(bufferin,buffer,sizein,&newsize_lzw);
		rlepack(bufferin,sizein,buffer2,&newsize_rle);

		mode = 0;

		// Note : only lzw mode for this project.
		//if(newsize_rle<sizein && newsize_rle< newsize_lzw)
		//	mode=1; //rle

		if( newsize_lzw >= 0 && newsize_lzw<sizein && newsize_lzw< newsize_rle)
			mode=2; //lzw

		switch(mode)
		{
			case 0:
				memcpy((buffer2+1),bufferin,sizein);
				buffer2[0]=0x0;
				memcpy(bufferout,buffer2,sizein+1);
				*sizeout=sizein+1;
			break;

			case 1:
				rlepack(bufferin,sizein,buffer+1,(int*)&newsize);
				buffer[0]=0x2;
				memcpy(bufferout,buffer,newsize+1);
				*sizeout=newsize+1;
			break;

			case 2:
				newsize = sizein * 10;
				lzw_compress(bufferin,buffer+1,sizein,(int*)&newsize);
				buffer[0]=0x1;
				memcpy(bufferout,buffer,newsize+1);
				*sizeout=newsize+1;
			break;

			case 3:
				newsize = sizein * 10;
				rlepack(bufferin,sizein,buffer2,(int*)&newsize);
				lzw_compress(buffer2,buffer+1,newsize,(int*)&newsize_lzw);
				buffer[0]=0x3;
				memcpy(bufferout,buffer,newsize_lzw+1);
				*sizeout=newsize_lzw+1;
			break;
		}

		free(buffer);
		free(buffer2);
	}

	return mode;
};


int main(int argc, char* argv[])
{

	bmpinfo infoo;
	unsigned char * dbuffer;
	unsigned char * cbuffer;
	int size,i,mode;

	printf("Binary2Header V0.6 - (c)HxC2001\n");

	memset(&infoo,0,sizeof(bmpinfo));

	if(argc==1)
		printf("Usage:\n");
	else
	{
		i=argc-1;
		do{
			if(strcmp("-BMP8",argv[i])==0)  infoo.type=8;
			if(strcmp("-BMP8P",argv[i])==0) infoo.type=9;
			if(strcmp("-BMP1",argv[i])==0)  infoo.type=1;
			if(strcmp("-DATA",argv[i])==0)  infoo.type=0xff;
			i--;
		}while(i);

		dbuffer = (unsigned char *)extractbmpdata(argv[1],&infoo);

		if(dbuffer!=NULL)
		{
			cbuffer=(unsigned char *)malloc(infoo.size+100+1024);
			if(cbuffer!=NULL)
			{
				memset(cbuffer,0,infoo.size+100+1024);

				mode = mi_pack(dbuffer,infoo.size,cbuffer, &size);

				printf("Generate header file... (Pack mode : %d, Source size : %d bytes, Final data size : %d bytes)\n",mode,infoo.size,size);

				infoo.csize=size;
				buildincludefile(argv[1],&infoo,cbuffer);

				free(cbuffer);
				free(dbuffer);
			}
			else
			{
				printf("Malloc Error!\n");
			}
		}
	}
	return 0;
}

