//////////////////////////////
//
//
// Created by Binary2Header V0.5
// (c) HxC2001
// (c) PowerOfAsm
//
// File: font8x8.bmp  Size: 2048  (2049) x:128 y:128
//
//


#ifndef BMAPTYPEDEF
#define BMAPTYPEDEF

typedef  struct _bmaptype
{
   int type;
   int Xsize;
   int Ysize;
   int size;
   int csize;
   unsigned char * data;
}bmaptype __attribute__ ((aligned (2)));

#endif


unsigned char __attribute__ ((aligned (2))) data_bmpfont8x8_bmp[]={
0x00,0xff,0xff,0xff,0xe0,0x07,0xe0,0x07,0xff,0x00,0xe7,0x00,0xff,0x00,0xff,0x00,0x00,0xff,0xff,0xff,0xe0,0x07,0xe0,0x07,0xff,0x00,0xc3,0x00,0xc7,0x00,0xff,0x00,
0x00,0xff,0xe0,0x07,0xe0,0x07,0xe0,0x07,0x00,0x00,0x99,0x00,0x83,0x00,0xff,0x00,0x00,0xff,0xe0,0x07,0xe0,0x07,0xe0,0x07,0x00,0x00,0x3c,0x00,0x81,0x00,0xff,0x00,
0x00,0xff,0xe0,0x07,0xe0,0x07,0xe0,0x07,0x00,0x00,0x99,0x00,0x81,0x00,0xff,0x00,0x00,0xff,0xe0,0x07,0xe0,0x07,0xe0,0x07,0x00,0x00,0xc3,0x00,0x81,0x00,0xff,0x00,
0x00,0xff,0xe0,0x07,0xff,0xff,0xe0,0x07,0x00,0xff,0xe7,0x00,0x81,0x00,0xff,0x00,0x00,0xff,0xe0,0x07,0xff,0xff,0xe0,0x07,0x00,0xff,0xff,0x00,0xff,0x00,0xff,0x00,
0x40,0x40,0xe7,0x3c,0x10,0x66,0xfe,0xfe,0x3c,0x10,0x10,0x10,0x87,0x87,0x87,0x87,0x60,0x60,0x81,0x18,0x10,0x18,0xf8,0x44,0x42,0x10,0x10,0x38,0xc5,0xc5,0xc7,0xc7,
0x70,0x70,0x81,0x99,0x10,0x18,0xe0,0x54,0x85,0x10,0x38,0x54,0xe2,0xe2,0xe2,0xe2,0x78,0x78,0x00,0xe7,0x5c,0x18,0xc0,0x38,0x89,0xfe,0xee,0xee,0xf5,0xf7,0xf5,0xf7,
0x7c,0x7c,0x00,0xe7,0x3e,0x18,0xc0,0x28,0x91,0x10,0x38,0x54,0xe7,0xe7,0xe7,0xe7,0x7e,0x70,0x81,0x99,0x3e,0x18,0x80,0x44,0xa1,0x10,0x10,0x38,0xb0,0xb0,0xb0,0xb0,
0x78,0x58,0x81,0x18,0x1c,0x18,0x80,0x54,0x42,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x6c,0x08,0xe7,0x3c,0x1c,0x66,0x00,0xfe,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x6c,0x6c,0x18,0x62,0x3c,0x18,0x0c,0x30,0x08,0x00,0x00,0x00,0x00,0x00,
0x00,0x18,0x6c,0xfe,0x3c,0x64,0x66,0x18,0x18,0x18,0x1c,0x18,0x00,0x00,0x00,0x06,0x00,0x18,0x24,0x6c,0x58,0x08,0x3c,0x08,0x30,0x0c,0x08,0x18,0x00,0x00,0x00,0x0c,
0x00,0x18,0x00,0x6c,0x3c,0x10,0x68,0x00,0x30,0x0c,0x14,0x3c,0x00,0x3c,0x00,0x18,0x00,0x00,0x00,0xfe,0x1a,0x26,0x66,0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x18,0x30,
0x00,0x18,0x00,0x6c,0x3c,0x46,0x3c,0x00,0x0c,0x30,0x00,0x18,0x18,0x00,0x18,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x18,0x3c,0x3c,0x0c,0x7e,0x3c,0x7e,0x3c,0x3c,0x00,0x00,0x00,0x00,0x00,0x3c,
0x66,0x38,0x66,0x66,0x1c,0x60,0x60,0x06,0x66,0x66,0x18,0x18,0x0c,0x00,0x30,0x66,0x6e,0x18,0x06,0x0c,0x3c,0x7c,0x7c,0x0c,0x3c,0x3e,0x18,0x18,0x18,0x3c,0x18,0x0c,
0x76,0x18,0x1c,0x06,0x6c,0x06,0x66,0x18,0x66,0x06,0x00,0x00,0x30,0x00,0x0c,0x18,0x66,0x18,0x30,0x66,0x7e,0x66,0x66,0x30,0x66,0x66,0x18,0x18,0x18,0x3c,0x18,0x00,
0x3c,0x3c,0x7e,0x3c,0x0c,0x3c,0x3c,0x30,0x3c,0x3c,0x18,0x18,0x0c,0x00,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x3c,0x7c,0x3c,0x7c,0x7e,0x7e,0x3c,0x66,0x7e,0x1e,0x66,0x60,0x62,0x66,0x3c,
0x42,0x66,0x66,0x66,0x66,0x60,0x60,0x66,0x66,0x18,0x06,0x6c,0x60,0x76,0x76,0x66,0x5a,0x7e,0x7c,0x60,0x66,0x78,0x78,0x60,0x7e,0x18,0x06,0x78,0x60,0x7e,0x7e,0x66,
0x5c,0x66,0x66,0x60,0x66,0x60,0x60,0x6e,0x66,0x18,0x06,0x6c,0x60,0x6a,0x6e,0x66,0x40,0x66,0x66,0x66,0x66,0x60,0x60,0x66,0x66,0x18,0x66,0x66,0x60,0x62,0x66,0x66,
0x3c,0x66,0x7c,0x3c,0x7c,0x7e,0x60,0x3e,0x66,0x7e,0x3c,0x66,0x7e,0x62,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0x3c,0x7c,0x3c,0x7e,0x66,0x66,0x62,0x66,0x66,0x7e,0x3c,0x00,0x3c,0x10,0x00,
0x66,0x66,0x66,0x60,0x18,0x66,0x66,0x62,0x3c,0x66,0x06,0x30,0x60,0x0c,0x28,0x00,0x7c,0x66,0x7c,0x3c,0x18,0x66,0x66,0x6a,0x18,0x3e,0x0c,0x30,0x30,0x0c,0x44,0x00,
0x60,0x66,0x66,0x06,0x18,0x66,0x66,0x7e,0x3c,0x06,0x18,0x30,0x18,0x0c,0x00,0x00,0x60,0x6e,0x66,0x66,0x18,0x66,0x3c,0x76,0x66,0x66,0x30,0x30,0x0c,0x0c,0x00,0x00,
0x60,0x3e,0x66,0x3c,0x18,0x3e,0x18,0x62,0x66,0x3c,0x7e,0x3c,0x06,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x60,0x00,0x06,0x00,0x3c,0x00,0x60,0x18,0x06,0x60,0x18,0x00,0x00,0x00,
0x0c,0x3c,0x60,0x00,0x06,0x3c,0x66,0x00,0x60,0x00,0x00,0x6c,0x18,0x00,0x00,0x00,0x00,0x06,0x7c,0x3e,0x3e,0x66,0x70,0x3c,0x7c,0x18,0x06,0x78,0x18,0x34,0x7c,0x3c,
0x00,0x3e,0x66,0x60,0x66,0x7e,0x60,0x66,0x66,0x18,0x06,0x6c,0x18,0x6a,0x66,0x66,0x00,0x66,0x66,0x60,0x66,0x60,0x60,0x3c,0x66,0x18,0x06,0x66,0x18,0x62,0x66,0x66,
0x00,0x3e,0x7c,0x3e,0x3e,0x3c,0x60,0x06,0x66,0x18,0x66,0x66,0x18,0x62,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x00,0x00,0x3c,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x10,0x30,0x14,0x00,
0x00,0x00,0x00,0x3c,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x08,0x28,0x00,0x7c,0x3e,0x7c,0x60,0x78,0x66,0x66,0x62,0x66,0x66,0x7e,0x10,0x10,0x08,0x00,0x00,
0x66,0x66,0x66,0x3c,0x60,0x66,0x66,0x62,0x38,0x66,0x0c,0x20,0x10,0x04,0x00,0x00,0x7c,0x3e,0x60,0x06,0x66,0x66,0x3c,0x6a,0x1c,0x3e,0x30,0x10,0x10,0x08,0x00,0x00,
0x60,0x06,0x60,0x3c,0x3c,0x3e,0x18,0x34,0x66,0x06,0x7e,0x10,0x10,0x08,0x00,0x00,0x60,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x00,0x0c,0x00,0x30,0x00,0x00,
0x1c,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x14,0x00,0x32,0x00,0x00,0x1a,0x00,0x00,0x18,0x18,0x10,0xd0,0x08,0x00,0x7f,0x00,0x08,0x00,
0x78,0x00,0x00,0x18,0x00,0x00,0x7e,0x7e,0x28,0xd0,0x3c,0x0c,0xcc,0x00,0x00,0x00,0x30,0x00,0x00,0x18,0x00,0x00,0x18,0x18,0x00,0x20,0x60,0x18,0xcf,0x00,0x7e,0x00,
0x78,0x00,0x00,0x3c,0x00,0x00,0x18,0x18,0x00,0x20,0x3c,0x30,0xcc,0x00,0x0c,0x00,0x32,0x00,0x18,0x18,0x6c,0xdb,0x18,0x7e,0x00,0x5b,0x06,0x18,0xcc,0x00,0x30,0x00,
0x1c,0x00,0x18,0x58,0x6c,0xdb,0x18,0x18,0x00,0x5b,0x3c,0x0c,0x7f,0x00,0x7e,0x00,0x00,0x00,0x08,0x30,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x00,0x36,0x00,0x10,0x18,0x48,0x6c,0x00,0x00,0x00,0x14,0xea,0x08,0x30,0x00,0x00,0x14,0x36,
0x00,0x18,0x18,0x6c,0x6c,0x18,0x00,0x00,0x28,0x55,0x3c,0x18,0x6c,0x00,0x08,0x00,0x00,0x18,0x08,0x6c,0x24,0x3c,0x00,0x00,0x00,0x55,0x60,0x0c,0x92,0x00,0x7e,0x66,
0x00,0x00,0x00,0x00,0x00,0x3c,0x7e,0xff,0x00,0x00,0x3c,0x0c,0x9e,0x00,0x0c,0x66,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x06,0x18,0x90,0x00,0x30,0x3e,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x30,0x6e,0x00,0x7e,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x08,0x00,0x3c,0x3c,0x00,0x00,0x00,0x3c,0x00,0x00,0x18,0x08,0x3c,0x5a,0x66,0x18,0x14,0x6c,0x42,0x06,0x00,0x7e,0x00,0x42,0x7e,
0x00,0x00,0x3e,0x66,0x24,0x3c,0x18,0x10,0x6c,0x99,0x3e,0x33,0x06,0x00,0xb1,0x00,0x00,0x18,0x68,0x30,0x42,0x7e,0x00,0x28,0x00,0xa1,0x66,0x66,0x00,0x00,0xa9,0x00,
0x00,0x18,0x68,0x60,0x42,0x18,0x18,0x14,0x00,0xa1,0x3e,0xcc,0x00,0x70,0xb1,0x00,0x00,0x18,0x3e,0x60,0x24,0x7e,0x18,0x08,0x00,0x99,0x00,0x66,0x00,0x00,0xa9,0x00,
0x00,0x18,0x08,0x7e,0x5a,0x18,0x18,0x28,0x00,0x42,0x00,0x33,0x00,0x00,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x3c,0x00,0x00,0x00,0x00,0x3c,0x00,
0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x40,0x40,0xc0,0x00,0x20,0x18,0x48,0x48,0x18,0x66,0x3e,0x00,0x00,0x60,0x18,0x00,0xc2,0xc2,0x22,0x18,
0x50,0x3c,0x10,0x10,0x30,0x66,0x7a,0x00,0x00,0x20,0x24,0xcc,0x44,0x44,0x44,0x00,0x20,0x18,0x20,0x48,0x00,0x76,0x7a,0x18,0x00,0x70,0x24,0x66,0xe8,0xe8,0x28,0x18,
0x00,0x00,0x78,0x30,0x00,0x6e,0x3a,0x18,0x00,0x00,0x18,0x33,0x16,0x16,0xd6,0x30,0x00,0x3c,0x00,0x00,0x00,0x60,0x1a,0x00,0x10,0x00,0x00,0x66,0x2a,0x21,0x2a,0x66,
0x00,0x00,0x00,0x00,0x00,0x60,0x1a,0x00,0x08,0x00,0x00,0xcc,0x4f,0x46,0x4f,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x02,0x07,0x02,0x00,
0x18,0x18,0x18,0x14,0x28,0x08,0x00,0x00,0x18,0x18,0x18,0x28,0x18,0x18,0x18,0x28,0x0c,0x30,0x24,0x28,0x00,0x14,0x7f,0x3c,0x0c,0x30,0x24,0x00,0x0c,0x30,0x24,0x00,
0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0xcc,0x62,0x7e,0x7e,0x7e,0x7e,0x3c,0x3c,0x3c,0x3c,0x66,0x66,0x66,0x66,0x66,0x66,0xff,0x60,0x60,0x60,0x60,0x60,0x18,0x18,0x18,0x18,
0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0xcc,0x62,0x78,0x78,0x78,0x78,0x18,0x18,0x18,0x18,0x66,0x66,0x66,0x66,0x66,0x66,0xcc,0x3c,0x60,0x60,0x60,0x60,0x18,0x18,0x18,0x18,
0x66,0x66,0x66,0x66,0x66,0x66,0xcf,0x10,0x7e,0x7e,0x7e,0x7e,0x3c,0x3c,0x3c,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x14,0x18,0x18,0x18,0x14,0x28,0x00,0x00,0x18,0x18,0x18,0x28,0x18,0x00,0x00,0x7c,0x28,0x0c,0x30,0x24,0x28,0x00,0x00,0x3c,0x0c,0x30,0x24,0x00,0x30,0x60,0x3c,
0x66,0x66,0x3c,0x3c,0x3c,0x3c,0x3c,0x42,0x67,0x66,0x66,0x66,0x66,0x66,0x7c,0x66,0xf6,0x76,0x66,0x66,0x66,0x66,0x66,0x24,0x6e,0x66,0x66,0x66,0x66,0x66,0x66,0x7c,
0x66,0x7e,0x66,0x66,0x66,0x66,0x66,0x18,0x76,0x66,0x66,0x66,0x66,0x3e,0x66,0x66,0x66,0x6e,0x66,0x66,0x66,0x66,0x66,0x24,0xe6,0x66,0x66,0x66,0x66,0x06,0x7c,0x66,
0x7c,0x66,0x3c,0x3c,0x3c,0x3c,0x3c,0x42,0x3c,0x3e,0x3e,0x3e,0x3e,0x3c,0x60,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x18,0x18,0x18,0x14,0x28,0x10,0x00,0x00,0x18,0x18,0x18,0x28,0x18,0x18,0x18,0x00,0x0c,0x30,0x24,0x28,0x00,0x28,0x00,0x00,0x0c,0x30,0x24,0x00,0x0c,0x30,0x24,0x28,
0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0xe6,0x1c,0x3c,0x3c,0x3c,0x3c,0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x19,0x20,0x66,0x66,0x66,0x66,0x18,0x18,0x18,0x18,
0x3e,0x3e,0x3e,0x3e,0x3e,0x3e,0x7f,0x20,0x7e,0x7e,0x7e,0x7e,0x18,0x18,0x18,0x18,0x66,0x66,0x66,0x66,0x66,0x66,0x98,0x1c,0x60,0x60,0x60,0x60,0x18,0x18,0x18,0x18,
0x3e,0x3e,0x3e,0x3e,0x3e,0x3e,0x77,0x08,0x3c,0x3c,0x3c,0x3c,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x14,0x18,0x18,0x18,0x14,0x00,0x00,0x00,0x18,0x18,0x18,0x00,0x18,0x00,0x00,0x48,0x28,0x0c,0x31,0x24,0x28,0x28,0x00,0x00,0x0c,0x30,0x24,0x28,0x30,0x00,0x28,
0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x7c,0x3c,0x3c,0x3c,0x3c,0x3c,0x00,0x3c,0x66,0x66,0x66,0x66,0x66,0x60,0x66,
0x1c,0x66,0x66,0x66,0x66,0x66,0x66,0x7e,0x6e,0x66,0x66,0x66,0x66,0x66,0x7c,0x66,0x36,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x76,0x66,0x66,0x66,0x66,0x3e,0x66,0x3e,
0x1c,0x66,0x3c,0x3c,0x3c,0x3c,0x3c,0x18,0x3c,0x3e,0x3e,0x3e,0x3e,0x06,0x7c,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x3c,0x60,0x3c,
0xcd};


static bmaptype bitmap_font8x8_bmp[]=
{
 1,
 128,
 128,
 2048,
 2049,
 data_bmpfont8x8_bmp
};
