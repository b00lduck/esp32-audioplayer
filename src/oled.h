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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Oled {

  private:
    uint8_t i2cAddress;
    Adafruit_SSD1306 ssd1306;

  public:
    Oled(uint8_t i2cAddress);
    void init();
    void clear();
    void trackName(char* trackName);
    void buttons(char buttons);
    void cardId(byte *card, uint8_t len);
    void fatalErrorMessage(char* error, char* info);
    void loadingBar(uint8_t percent);
    void volumeBar(uint8_t percent);
};
