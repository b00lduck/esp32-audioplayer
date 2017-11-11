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
#include "sd.h"
#include "tools.h"

SDCard::SDCard(CSMultiplexer *_csMux, uint8_t _csAddress) : 
  csAddress(_csAddress),
  csMux(_csMux)  
  {}

bool SDCard::init() {

  if (sd.begin(
        [&](){
          csMux->chipSelect(SD_CS_ADDRESS);
        },
        [&](){
          csMux->chipDeselect();
        }, 
        SD_SCK_MHZ(50))){
     Serial.println(F("SD Card initialized."));
     printDirectory();
     return true;
  }
  
  return false;      
}

void SDCard::printDirectory() {

  SdFile dirFile;
  if (!dirFile.open("/", O_READ)) {
    Serial.println(F("open root failed"));
    return;
  }
    
  SdFile file;  
  while (file.openNext(&dirFile, O_READ)) {
    if (!file.isSubDir() && !file.isHidden()) {
      file.printName(&Serial);
      Serial.println();
    }
    file.close();
  }

  dirFile.close();
}
