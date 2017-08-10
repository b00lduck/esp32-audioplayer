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

class CSMultiplexer {

  private:
    uint8_t gpio_a0;
    uint8_t gpio_a1;
    uint8_t gpio_a2;

    uint8_t selected;

  public:
    CSMultiplexer(uint8_t gpio_a0, uint8_t gpio_a1, uint8_t gpio_a2);
    void init();
    void chipSelect(uint8_t address);
    void chipDeselect();
    
};
