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
#include "sd.h"
#include <FS.h>
#include "config.h"

SDCard::SDCard(uint8_t _csPin) : 
  csPin(_csPin) 
  {}

bool SDCard::init() {
  if (SD.begin(csPin, SPI, 4000000)){
     Serial.println(F("[SD-Card] media initialized"));
     assureDirectory("/upload");    
     printDirectory();
  }  
  return false;      
}

void SDCard::printDirectory() {
  
    Serial.println("Listing directory");

    File root = SD.open("/");
    if(!root){
        Serial.println("Failed to open root directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
