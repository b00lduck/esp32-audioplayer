/**
 * 
 * Copyright 2018-2020 D.Zerlett <daniel@zerlett.eu>
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
#include "mapper.h"
#include "config.h"
#include "FS.h"
#include "sd.h"
#include "playlist.h"
#include "../utils.h"

Mapper::MapperError Mapper::init() {
    SDCard::assureDirectory("/cards");
    return MapperError::OK;
}

Mapper::MapperError Mapper::createPlaylist(Playlist *playlist, const char *path) {
  
  File dir = SD.open(path, FILE_READ);
  if(!dir){
      return OK;
  }

  File file = dir.openNextFile();
  uint8_t n = 0;
  while(file){
      if(!file.isDirectory()){
          char entry[MAX_PLAYLIST_ENTRY_LENGTH];
          snprintf(entry, MAX_PLAYLIST_ENTRY_LENGTH, file.name() + strlen(CARDS_DIRECTORY) + CARD_ID_STRING_LENGTH + 1);

          // omit mapping file
          if (strncmp(entry, META_FILE_NAME, strlen(META_FILE_NAME)) != 0) {
            playlist->addEntry(file.name());
          }
      }
      file = dir.openNextFile();
  }
  playlist->sort();
  return OK;
}

Mapper::MapperError Mapper::readMetaFile(char cardIdString[CARD_ID_STRING_LENGTH], MappingMeta *meta) {

  char metaFilePath[META_FILE_PATH_LENGTH];
  snprintf(metaFilePath, META_FILE_PATH_LENGTH, "%s/%8s/%s", CARDS_DIRECTORY, cardIdString, META_FILE_NAME);

  // set card ID
  strncpy(meta->cardId, cardIdString, CARD_ID_STRING_LENGTH);
  
  // default name is card ID
  strncpy(meta->name, cardIdString, CARD_ID_STRING_LENGTH);

  Serial.printf("Reading meta file %s\n", metaFilePath);

  File metaFile = SD.open(metaFilePath, FILE_READ);
  if (metaFile) {
    uint8_t nameLen = readNameFromMetaFile(meta->name, &metaFile);
    // if no name could be read, use card ID as name  
    if (nameLen == 0) {
      Serial.println(F("Card name not found. Using card ID as name."));
      strncpy(meta->name, cardIdString, CARD_ID_STRING_LENGTH);
    }
  }
  Serial.printf("Card %s name is: %s\n", cardIdString, meta->name);
  
  return MapperError::OK;
}

uint16_t Mapper::readNameFromMetaFile(char str[MAX_CARD_NAME_STRING_LENGTH], File *stream) {
  uint16_t i = 0;  
  memset(str, 0, MAX_CARD_NAME_STRING_LENGTH); 
  while (i < (MAX_CARD_NAME_STRING_LENGTH - 1)) {   
    int16_t ch = stream->read();
    if ((ch < 0) || (ch == 10)) {
      break;
    }
    str[i++] = ch;
  }
  return i; 
}

/**
 * Create mapping iterator to be used from the HTTP interface to get al list of available mappings.
 */
Mapper::MapperError Mapper::createMappingIterator(File *it) {
  *it = SD.open(CARDS_DIRECTORY, FILE_READ);
  if (!*it) {    
    return CARDS_DIRECTORY_NOT_FOUND;
  } 
  return OK;
}

/**
 * Get next mapping entry from directory handle
 */
Mapper::MapperError Mapper::nextMapping(File *it, Mapper::Mapping *mapping) {
     
  while(true) {
    File entry = it->openNextFile();
    if (!entry) {
      return NO_MORE_CARDS;  
    }

    if (entry.isDirectory()) {

      char name[CARD_ID_STRING_LENGTH];
      strncpy(name, entry.name() + strlen(CARDS_DIRECTORY) + 1, CARD_ID_STRING_LENGTH);
      MapperError err;
      
      err = readMetaFile(name, &mapping->mappingMeta);
      if (err != OK) {
        Serial.println("Error 42");
        return err;
      }

      err = createPlaylist(&mapping->playlist, entry.name());
      if (err != OK) {
        Serial.println("Error 43");
        return err;
      }

      return OK;
    }
  }
}

/**
 * Create directory iterator to be used from the HTTP interface to get al list of all files and directories in the given path.
 */
Mapper::MapperError Mapper::createFileIterator(File *it, const char *dir) {  
  *it = SD.open(dir, FILE_READ);
  if (!*it) {    
    return DIRECTORY_NOT_FOUND;
  } 
  return OK;
}

/**
 * Get next entry from directory handle
 */
Mapper::MapperError Mapper::nextFile(File *it, char name[MAX_FILENAME_STRING_LENGTH], char type[16]) {
     
  while(true) {
    File entry = it->openNextFile();
    if (!entry) {
      return NO_MORE_FILES;  
    }

    strncpy(name, entry.name(), MAX_FILENAME_STRING_LENGTH);
    if (entry.isDirectory()) {
      strncpy(type, "directory", 16);
    } else {
      
      if (
        filenameHasExtension(entry.name(), "mp3") || 
        filenameHasExtension(entry.name(), "wav") ||
        filenameHasExtension(entry.name(), "wma")
      ) {
        strncpy(type, "file_audio", 16);
      } else if (
        filenameHasExtension(entry.name(), "txt")
      ) {
        strncpy(type, "file_text", 16);
      } else {
        strncpy(type, "file_other", 16);
      }      
      
    }
    return OK;
  }
}