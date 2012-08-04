/*
//
// Copyright (C) 2009, 2010, 2011, 2012 Jean-François DEL NERO
//
// This file is part of the HxCFloppyEmulator file selector.
//
// HxCFloppyEmulator file selector may be used and distributed without restriction
// provided that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// HxCFloppyEmulator file selector is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// HxCFloppyEmulator file selector is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HxCFloppyEmulator file selector; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/


//#define IJ_DEBUG



#include <string.h>

#include "atari_hw.h"
#include "hxcfeda.h"

#ifdef IJ_DEBUG
	#include "gui_utils.h" // DEBUG ONLY !
	static int debug_line=0;
#endif


//#include "cfg_file.h"

//#include "dir.h"
#include "filelist.h"


//#include "conf.h"



// constants
#define IJ_TIMEOUT 2000    // maximum time between two keystrokes (in ms)
#define IJ_MAXLEN  15      // maximum number of chars


// static variables:
static unsigned long _lastTime = 0;
static char _searchString[IJ_MAXLEN+1];
static unsigned char _isValid;						// previous search matched

// interval:
static UWORD _mini;						// low, inclusive
static UWORD _maxi;						// high, exclusive
static UBYTE _phase;					// 0:directories, 1:files


/**
 * If a key was stroke within a short time, add that key to the current search
 * otherwise, initiate an other instajump.
 * this function does not call the search function
 */
void ij_keyEvent(signed char key)
{
	unsigned long time;
	int len = strlen(_searchString);
	UWORD firstFile;

	time = get_hz200();
	if ( (time - _lastTime) < (IJ_TIMEOUT / 5)  ) {
		if ('\0' == key) {
			_mini++;
			key = _searchString[len-1];
			_searchString[0] = '\0';
			len = 0;
			_phase--;
			if (!_phase) {
				_maxi    = firstFile;
			} else {
				_maxi    = fli_getNbEntries();
			}
			if (_mini < _maxi) {
				_isValid = 1;
			}
		}
	} else {
		firstFile = fli_getFirstFile();

		// init a new search
		len = 0;
		_isValid = 1;
		_mini    = 0;

		if (0 == firstFile) {
			_phase   = 1;		// files
			_maxi    = fli_getNbEntries();
		} else {
			_phase   = 0;		// directories
			_maxi    = firstFile;
		}
		#ifdef IJ_DEBUG
			debug_line = 0;
		#endif
	}

	if (_isValid && (len < IJ_MAXLEN)) {
		_searchString[len++] = key | 32;	// lower case
		_searchString[len]   = '\000';
	}

	_lastTime = time;
}

void ij_clear()
{
	_lastTime = 0;
}


/**
 * perform the search
 * @returns the entry number
 */
UWORD ij_performSearch()
{
	UWORD curFile;
	UWORD lastok = 0xffff;
	UWORD lastmaxi;
	UWORD backupmini;
	struct fs_dir_ent dir_entry;
	int cmp;

	if (!_isValid) {
		return 0xffff;
	}

	backupmini = _mini;
	lastmaxi = _maxi;

	#ifdef IJ_DEBUG
		hxc_printf(0, 0, 8*(debug_line++), "searching for %s in [%d;%d[", _searchString, _mini, _maxi);
	#endif

	do {
		curFile = (_mini + _maxi) >> 1;
		fli_getDirEntryMSB(curFile, &dir_entry);

		mystrlwr(dir_entry.filename);
		cmp = strncmp(dir_entry.filename, _searchString, strlen(_searchString));

		#ifdef IJ_DEBUG
			hxc_printf(0, 0, 8*(debug_line++), "try p=%d in [%d;%d[ index %d (%d):%s", _phase, _mini, _maxi, curFile, cmp, dir_entry.filename);
		#endif

		if (curFile == _mini) {
			//last
			if (cmp != 0 && 0xffff != lastok) {
				cmp = 0;
				curFile = lastok;
			}
			if (0 == cmp) {
				// found
				_mini = curFile;
				_maxi = lastmaxi;
				#ifdef IJ_DEBUG
					hxc_printf(0, 0, 8*(debug_line++), "Found at %d in [%d;%d[", curFile, _mini, _maxi);
				#endif
				return curFile;
			}

			// not found
			_phase++; // files
			if (1 == _phase) {
				_mini    = fli_getFirstFile();
				_maxi    = fli_getNbEntries();
				lastmaxi = _maxi;
			} else {
				_mini = backupmini;
			}
		} else {
			if ( cmp < 0) {
				_mini = curFile;
			} else {
				_maxi = curFile;
				if (0 == cmp) {
					// this entry matches, but it is not necessarely the first one, so, continue to search.
					// remember that this entry was OK, in case it was actually the first one to match.
					lastok = curFile;
				} else {
					lastmaxi = curFile;
				}
			}
		}
		curFile++;
	} while (_phase < 2);

	#ifdef IJ_DEBUG
		hxc_printf(0, 0, 8*(debug_line++), "Not found in [%d;%d[", _mini, _maxi);
	#endif
	_isValid = 0;
	return 0xffff;
}