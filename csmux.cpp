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
#include <Wire.h>
#include "csmux.h"

CSMultiplexer::CSMultiplexer(uint8_t _sda, uint8_t _scl) : 
  sda(_sda), 
  scl(_scl),
  selected(255) {}

void CSMultiplexer::init() {
  Wire.begin(sda, scl);
  Wire.setClock(600000L);
  
  Wire.beginTransmission(0x20);
  Wire.write(0x00); // IODIRA 
  Wire.write(0x00); // PORTA output
  Wire.endTransmission();
  
  //Wire.beginTransmission(0x20);
  //Wire.write(0x01); // IODIRB
  //Wire.write(0x00); // PORTB output
  //Wire.endTransmission();  

  Wire.beginTransmission(0x20);
  Wire.write(0x12); // PORTA
  Wire.write(0xff); // value
  Wire.endTransmission();    
  
  //Wire.beginTransmission(0x20);
  //Wire.write(0x13); // PORTB
  //Wire.write(0b11000000); // value
  //Wire.endTransmission();    
}

void CSMultiplexer::chipSelect(uint8_t address) {    
  if (selected != address) {
    uint8_t value = 0xff - (1 << address);    
    Wire.beginTransmission(0x20);
    Wire.write(0x12);
    Wire.write(value);
    Wire.endTransmission();    
    selected = address;
  }
}

void CSMultiplexer::chipDeselect() {
    Wire.beginTransmission(0x20);
    Wire.write(0x12);
    Wire.write(0xff);
    Wire.endTransmission();
    selected = 255;
}
    

