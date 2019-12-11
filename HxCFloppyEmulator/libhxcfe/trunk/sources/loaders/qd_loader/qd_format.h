/*
//
// Copyright (C) 2006-2019 Jean-Fran�ois DEL NERO
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

// All structure datas are packed !
#pragma pack(1)

typedef struct qdhfefileformatheader_
{
	unsigned char HEADERSIGNATURE[8]; //HXCQDDRV
	uint32_t formatrevision;
	uint32_t number_of_track; // 1
	uint32_t number_of_side;  // x
	uint32_t track_encoding;
	uint32_t write_protected;
	uint32_t bitRate;   // (cells rate / s)
	uint32_t flags;
	uint32_t track_list_offset;
} qdhfefileformatheader;

typedef struct qdtrack_
{
	uint32_t offset;
	uint32_t track_len;
	uint32_t start_sw_position;
	uint32_t stop_sw_position;
}qdtrack;


typedef struct MFMIMG_
{
	uint8_t  headername[7];          // "HXCMFM\0"

	uint16_t number_of_track;
	uint8_t  number_of_side;         // Number of elements in the MFMTRACKIMG array : number_of_track * number_of_side

	uint16_t floppyRPM;              // Rotation per minute.
	uint16_t floppyBitRate;          // 250 = 250Kbits/s, 300 = 300Kbits/s...
	uint8_t  floppyiftype;

	uint32_t mfmtracklistoffset;    // Offset of the MFMTRACKIMG array from the beginning of the file in number of uint8_ts.
}MFMIMG;

// Right after this header, the MFMTRACKIMG array is present.
// Number of element in the MFMTRACKIMG array : number_of_track * number_of_side
// Here is one MFMTRACKIMG element :

typedef struct MFMTRACKIMG_
{
	uint16_t track_number;
	uint8_t  side_number;
	uint32_t mfmtracksize;          // MFM/FM Track size in bytes
	uint32_t mfmtrackoffset;        // Offset of the track data from the beginning of the file in number of bytes.
}MFMTRACKIMG;

// After this array, track datas can be found.
// Each data byte must be sent to the floppy interface from the bit 7 to the bit 0.

#pragma pack()
