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
#include "mapper.h"
#include "config.h"
#include "FS.h"
#include "SD.h"

Mapper::MapperError Mapper::init() {
    return checkMappingFile();
}

/**
 * try to read mapping file line by line and try to match the first 8 characters,
 * which are interpreted as uppercase hex.
 */
Mapper::MapperError Mapper::resolveIdToFilename(byte id[4], char filename[MAX_FILENAME_STRING_LENGTH]) {

  File mappingFile = SD.open(MAPPING_FILE, FILE_READ);
  if (!mappingFile) {
    return MapperError::MAPPING_FILE_NOT_FOUND;    
  }

  // convert id to string
  char id_string[ID_STRING_LENGTH];
  uid_to_string(id, id_string);

  Serial.print("ID string: ");
  Serial.println(id_string);

  char line[MAX_MAPPING_LINE_STRING_LENGTH];
  size_t n;
  while ((n = readLine(line, &mappingFile)) > 0) {
    
    char found_id[ID_STRING_LENGTH];
    MapperError err = extractIdFromLine(found_id, line);
    if (err != MapperError::OK) {
      return err;
    }
    
    if (strncmp(found_id, id_string, 8) == 0) {
      strncpy(filename, &(line[ID_STRING_LENGTH]), MAX_FILENAME_STRING_LENGTH);
      return OK;
    }
    
    Serial.println(line);

  }

  filename[0] = 0;
  return ID_NOT_FOUND;
}

/**
 * Parse one line of the mapping file and convert the HEX, which is stored in the first 8 chars, to lowercase.
 * If any invalid chars are found (valid chars are [a-fA-F0-9]) the id "00000000" is returned.
 */
Mapper::MapperError Mapper::extractIdFromLine(char found_id[ID_STRING_LENGTH], char line[MAX_MAPPING_LINE_STRING_LENGTH]) {
  for(uint8_t i=0; i<8; i++) {
    // to lowercase
    if (line[i] >= 65 && line[i] <= 70) {
      line[i] +=32;
    }      
    if ((line[i] < 97 || line[i] > 102) && (line[i] < 48 && line[i] > 57)) {      
      return MapperError::MALFORMED_CARD_ID;
    }
    found_id[i] = line[i];
  }
  found_id[8] = 0;
}

// copy id to char array
void Mapper::uid_to_string(byte *uid, char output[ID_STRING_LENGTH]) {
  #if ID_BYTE_ARRAY_LENGTH == 4  
    sprintf(output, "%02x%02x%02x%02x", uid[0], uid[1], uid[2], uid[3]);
  #else
    #error Only 4 byte IDs supported
  #endif
}


uint16_t Mapper::readLine (char str[MAX_MAPPING_LINE_STRING_LENGTH], File *stream) {  
  uint16_t i = 0;  
  memset(str, 0, MAX_MAPPING_LINE_STRING_LENGTH); 
  while (i < (MAX_MAPPING_LINE_STRING_LENGTH - 1)) {   
    int16_t ch = stream->read();
    if ((ch < 0) || (ch == 10)) {
      break;
    }
    str[i++] = ch;
  }
  return i; 
}

/**
 * Check syntax of mapping file and existence of linked files
 */
Mapper::MapperError Mapper::checkMappingFile() {
  Serial.println("Checking mapping file...");
  File mappingFile = SD.open(MAPPING_FILE, FILE_READ);
  if (!mappingFile) {
    return MapperError::MAPPING_FILE_NOT_FOUND;    
  }
  Serial.println("Found mapping file...");

  char line[MAX_MAPPING_LINE_STRING_LENGTH];
  
  while ((readLine(line, &mappingFile)) > 0) {    
    Serial.printf("Line contents: %s\n", line);
    MapperError err = checkMappingLine(line);
    if (err != MapperError::OK) {
      return err;  
    }    
  }

  return MapperError::OK;
}


/**
 * Check syntax of mapping line and existence of linked file
 */
Mapper::MapperError Mapper::checkMappingLine(char* line) {

  // check minimum line length given an one charachter long filename
  if (strlen(line) < (ID_STRING_LENGTH + 1) ) {
    return MapperError::LINE_TOO_SHORT;
  }

  // check maximum line length by validating that last char is a newline
  if (strlen(line) >= MAX_MAPPING_LINE_STRING_LENGTH - 1) {
    return MapperError::LINE_TOO_LONG;
  }  

  // check format of hex id. [a-fA-F0-9]
  for(uint8_t i = 0; i<(ID_BYTE_ARRAY_LENGTH*2); i++) {
    char x = line[i];
    if (!((x >= 65 && x <= 70) || (x >= 97 && x <= 102) || (x >= 48 && x <= 57))) {
      return MapperError::MALFORMED_CARD_ID;
    }   
  }

  // check space
  if (line[ID_STRING_LENGTH-1] != 32) {
    return MapperError::MALFORMED_LINE_SYNTAX;
  }

  // check file entry
  char* filename = line + ID_STRING_LENGTH;

  // filename must start with a slash or a hash
  if (filename[0] != '/' && filename[0] != '#') {
    return MapperError::MALFORMED_FILE_NAME;
  }

  if (filename[0] == '#') {
    // special card, everything ok
  } else {
    // sound file, check if file exists
    Serial.println("try to open file");
    File dataFile = SD.open(filename, FILE_READ);   
    if (!dataFile) {
      return MapperError::REFERENCED_FILE_NOT_FOUND;
    }
    dataFile.close();  
  }  

  return MapperError::OK;
}
