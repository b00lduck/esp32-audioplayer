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
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include "config.h"

bool stringEndsWith(const char *str, const char *suffix) {
    if (!str || !suffix) return false;
    unsigned int lenstr = strlen(str);
    unsigned int lensuffix = strlen(suffix);
    if (lensuffix > lenstr) return false;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

void stringToUpper(char *str) {    
  for (unsigned int i = 0; str[i]!='\0'; i++) {
     if(str[i] >= 'a' && str[i] <= 'z') {
        str[i] = str[i] - 32;
     }  
  }
}

void stringToLower(char *str) {    
  for (unsigned int i = 0; str[i]!='\0'; i++) {
     if(str[i] >= 'A' && str[i] <= 'Z') {
        str[i] = str[i] + 32;
     }  
  }
}

bool filenameHasExtension(const char *filename, const char *extension) {
  char upperFilename[MAX_FILENAME_STRING_LENGTH];
  strncpy(upperFilename, filename, MAX_FILENAME_STRING_LENGTH);
  stringToUpper(upperFilename);

  char upperExtension[MAX_FILENAME_STRING_LENGTH];
  snprintf(upperExtension, MAX_FILENAME_STRING_LENGTH, ".%s", extension);
  stringToUpper(upperExtension);

  return stringEndsWith(upperFilename, upperExtension);
}