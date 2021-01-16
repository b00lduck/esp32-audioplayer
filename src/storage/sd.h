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
#include <SD.h>

class SDCard {

  public:  
    SDCard(uint8_t _csPin);
    bool init();

    static bool assureDirectory(const char path[]) {
        Serial.printf("[SD-Card] Assuring directory %s\n", path);
        File chk = SD.open(path);
        if(!chk){
            return SD.mkdir(path);
        } 
        if (!chk.isDirectory()) {
            Serial.println(F("[SD-Card] There is a file named 'upload' in the root of the SD card. Please remove this file from the card."));
            return false;
        }
        return true;
    }
    
  private:
    uint8_t csPin;

    void printDirectory();
};


