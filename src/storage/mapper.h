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

#define MAX_CARD_NAME_LENGTH                  32 
#define MAX_CARD_NAME_STRING_BUFFER_LENGTH    (MAX_CARD_NAME_LENGTH + 1) 
#define META_FILE_NAME                        "meta.txt"
#define CARDS_DIRECTORY                       "/cards"
#define META_FILE_PATH_LENGTH                 (6+1+8+1+8+1)
#define CARD_DIRECTORY_PATH_LENGTH            (6+1+8+1)

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
       * Directory not present on SD card
       */
      DIRECTORY_NOT_FOUND,

      /**
       * File not present on SD card
       */
      FILE_NOT_FOUND,

      /**
       * The desired ID was not found
       */
      CARD_ID_NOT_FOUND,

      /**
       * the meta file could not be found
       */
      META_FILE_NOT_FOUND,

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
      char cardId[CARD_ID_STRING_BUFFER_LENGTH];
      char name[MAX_CARD_NAME_STRING_BUFFER_LENGTH];
    };

    struct Mapping {
      MappingMeta mappingMeta;
      Playlist playlist;
    };  

    MapperError init();   

    MapperError createMappingIterator(File *file);
    MapperError nextMapping(File *file, Mapper::Mapping *mapping);
    MapperError createPlaylist(Playlist *playlist, const char cardIdString[CARD_ID_STRING_BUFFER_LENGTH]);

    MapperError initializeCard(const char cardIdString[CARD_ID_STRING_BUFFER_LENGTH], const char cardName[MAX_CARD_NAME_STRING_BUFFER_LENGTH]);
    MapperError writeNameToMetaFile(const char cardIdString[CARD_ID_STRING_BUFFER_LENGTH], const char cardName[MAX_CARD_NAME_STRING_BUFFER_LENGTH]);

    MapperError nextFile(File *it, char name[256], char type[16], size_t *size);

    MapperError readMetaFile(MappingMeta *meta, const char cardIdString[CARD_ID_STRING_BUFFER_LENGTH]);
    
  private:
    
    uint16_t readNameFromMetaFile(File *stream, char str[MAX_CARD_NAME_STRING_BUFFER_LENGTH]);
    bool stringEndsWith(const char *str, const char *suffix);

    void metaFilePath(char metaFilePath[META_FILE_PATH_LENGTH], const char cardIdString[CARD_ID_STRING_BUFFER_LENGTH]);
    void cardDirPath(char cardDirPath[CARD_DIRECTORY_PATH_LENGTH], const char cardIdString[CARD_ID_STRING_BUFFER_LENGTH]);    
};
