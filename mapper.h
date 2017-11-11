/**
 * 
 * Copyright 2017 D.Zerlett <daniel@zerlett.eu>
 * 
 * This file is part of esp8266-audioplayer.
 * 
 * esp8266-audioplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * esp8266-audioplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esp8266-audioplayer. If not, see <http://www.gnu.org/licenses/>.
 *  
 */
#include "Arduino.h"
#include "config.h"
#include <stdexcept>

#define MAPPING_FILE                    "mapping.txt"
#define ID_STRING_LENGTH                (ID_BYTE_ARRAY_LENGTH * 2 + 1)    // with zero terminator
#define MAX_FILENAME_STRING_LENGTH      (50 + 1)                          // with zero terminator
#define MAX_MAPPING_LINE_STRING_LENGTH  (MAX_FILENAME_STRING_LENGTH + ID_STRING_LENGTH)    

class Mapper {
  public:  
 
    enum MapperError {

      /**
       * all went OK
       */
      OK,
    
      /**
       * The desired ID file was not found in the mapping file
       */
      ID_NOT_FOUND,
      
      /**
       * The mapping file was not found
       */
      MAPPING_FILE_NOT_FOUND,
      
      /**
       * The file referenced in the mapping file was not found
       */
      REFERENCED_FILE_NOT_FOUND,
      
      /**
       * A line of the mapping file is too long or the last line is missing a newline
       */
      LINE_TOO_LONG,

      /**
       * A line of the mapping file is too short
       */
      LINE_TOO_SHORT,

      /**
       * Bad characters in card ID
       */
      MALFORMED_CARD_ID,

      /**
       * Space missing or at the wrong place
       */
      MALFORMED_LINE_SYNTAX
    };
        
    MapperError init();   
    MapperError resolveIdToFilename(byte id[ID_BYTE_ARRAY_LENGTH], char filename[MAX_FILENAME_STRING_LENGTH]); 
    
  private:
    void uid_to_string(byte *uid, char output[9]);
    MapperError extract_id_from_line(char found_id[ID_STRING_LENGTH], char line[MAX_MAPPING_LINE_STRING_LENGTH]);
    MapperError checkMappingFile();
    MapperError checkMappingLine(char* line);
};


