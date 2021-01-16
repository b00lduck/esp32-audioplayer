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
#pragma once

#define MAX_PLAYLIST_LENGTH 32
#define MAX_PLAYLIST_ENTRY_LENGTH 64

class Playlist { 

  public:  

    unsigned char numEntries = 0;
    unsigned currentEntryIndex = 0;
    char entries[MAX_PLAYLIST_LENGTH][MAX_PLAYLIST_ENTRY_LENGTH];

    Playlist();   
    void addEntry(const char *entry);
    void sort();
    char* getNextEntry();
};
