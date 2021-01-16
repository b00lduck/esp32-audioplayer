/**
 * 
 * Copyright 2018 D.Zerlett <daniel@zerlett.eu>
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
#include "Arduino.h"
#include "config.h"
#include "FS.h"
#include "playlist.h"

#define MAX_CARD_NAME_STRING_LENGTH    32 
#define META_FILE_NAME                 "meta.txt"
#define CARDS_DIRECTORY                "/cards"
#define META_FILE_PATH_LENGTH          (6+1+8+1+8+1)

class Mapper {
  public:  

    enum MapperError {

      /**
       * all went OK
       */
      OK,

      /**
       * Cards directory not present on SD card or is a file
       */
      CARDS_DIRECTORY_NOT_FOUND,

      /**
       * Root directory not present on SD card
       */
      DIRECTORY_NOT_FOUND,

      /**
       * The desired ID was not found
       */
      CARD_ID_NOT_FOUND,

      /**
       * the meta file could not be read
       */
      META_FILE_ERROR,      
            
      /**
       * No more card mappings to be read
       */
      NO_MORE_CARDS,

      /**
       * No more files to be read
       */
      NO_MORE_FILES

    };

    struct MappingMeta {
      char cardId[CARD_ID_STRING_LENGTH];
      char name[MAX_CARD_NAME_STRING_LENGTH];
    };

    struct Mapping {
      MappingMeta mappingMeta;
      Playlist playlist;
    };  

    MapperError init();   

    MapperError createMappingIterator(File *file);
    MapperError nextMapping(File *file, Mapper::Mapping *mapping);
    MapperError createPlaylist(Playlist *playlist, const char *path);

    MapperError createFileIterator(File *it, const char *dir);
    MapperError nextFile(File *it, char name[256], char type[16]);
    
  private:
    MapperError readMetaFile(char cardIdString[CARD_ID_STRING_LENGTH], MappingMeta *meta);
    uint16_t readNameFromMetaFile(char str[MAX_CARD_NAME_STRING_LENGTH], File *stream);
    bool stringEndsWith(const char *str, const char *suffix);
    
};


