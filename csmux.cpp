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

#include "csmux.h"

CSMultiplexer::CSMultiplexer(uint8_t _gpio_a0, uint8_t _gpio_a1, uint8_t _gpio_a2) : 
  gpio_a0(_gpio_a0), 
  gpio_a1(_gpio_a1),
  gpio_a2(_gpio_a2),
  selected(0) {}

void CSMultiplexer::init() {
  pinMode(gpio_a0, OUTPUT);
  pinMode(gpio_a1, OUTPUT);
  pinMode(gpio_a2, OUTPUT);
  digitalWrite (gpio_a0, LOW);
  digitalWrite (gpio_a1, LOW);
  digitalWrite (gpio_a2, LOW);
}

void CSMultiplexer::chipSelect(uint8_t address) {  
  if (selected != address) {
    digitalWrite (gpio_a0, address & 1 ? HIGH : LOW);        
    digitalWrite (gpio_a1, address & 2 ? HIGH : LOW);        
    digitalWrite (gpio_a2, address & 4 ? HIGH : LOW);        
    selected = address;
  }
}

void CSMultiplexer::chipDeselect() {
  chipSelect(0); 
}
    

