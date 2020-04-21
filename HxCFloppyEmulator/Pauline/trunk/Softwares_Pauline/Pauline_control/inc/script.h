/*
//
// Copyright (C) 2019-2020 Jean-François DEL NERO
//
// This file is part of the Pauline control software
//
// Pauline control software may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// Pauline control software is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// Pauline control software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Pauline control software; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/

#define MAX_LINE_SIZE 2048
#define DEFAULT_BUFLEN 512

int execute_script(char * filename);
int execute_line(char * line);
int savepinstate_script(char * filename);

// Output Message level
#define MSG_NONE                         0
#define MSG_INFO_0                       1
#define MSG_INFO_1                       2
#define MSG_WARNING                      3
#define MSG_ERROR                        4
#define MSG_DEBUG                        5

typedef int (* PRINTF_FUNC)(int MSGTYPE, char * string, ... );

void setOutputFunc( PRINTF_FUNC ext_printf );
