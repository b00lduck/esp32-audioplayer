/**
 * 
 * Copyright 2021 D.Zerlett <daniel@zerlett.eu>
 * 
 * This file is part of esp32-audioplayer.
 * 
 * esp32-audioplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * esp32-audioplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esp32-audioplayer. If not, see <http://www.gnu.org/licenses/>.
 *  
 */
#include <string.h>
#include <stdio.h>
#include "playlist.h"

Playlist::Playlist() : numEntries(0), currentEntryIndex(0) {}

/**
 * Add one entry to the list. Silently fail, if max size of list is exceeded.
 */
void Playlist::addEntry(const char *entry) {
  if (numEntries < MAX_PLAYLIST_LENGTH) {
    strncpy(entries[numEntries], entry, MAX_PLAYLIST_ENTRY_LENGTH);
    numEntries++;
  }
}

/**
 * Bubblesort
 */
void Playlist::sort() {
  bool swapped = false;
  do {
    for(unsigned char i=0;i<numEntries-1;i++) {      
      if(strncmp(entries[i], entries[i+1], MAX_PLAYLIST_ENTRY_LENGTH) > 0) {
        char buf[MAX_PLAYLIST_ENTRY_LENGTH];
        strncpy(buf, entries[i+1], MAX_PLAYLIST_ENTRY_LENGTH);
        strncpy(entries[i+1], entries[i], MAX_PLAYLIST_ENTRY_LENGTH);
        strncpy(entries[i], buf, MAX_PLAYLIST_ENTRY_LENGTH);
        swapped = true;
      }
    }
  } while (!swapped);
}

char* Playlist::getNextEntry() {
  if (currentEntryIndex >= numEntries) {
    return NULL;
  }

  char *entry = entries[currentEntryIndex];
  currentEntryIndex++;
  return entry;
}

void Playlist::reset() {
  currentEntryIndex = 0;
  numEntries = 0;
}