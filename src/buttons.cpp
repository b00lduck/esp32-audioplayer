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
#include "config.h"
#include "buttons.h"

Buttons::Buttons() : state(0), oldState(0) {}

void Buttons::init() {
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
}

bool Buttons::read() {

    state = digitalRead(BUTTON1) |  
            digitalRead(BUTTON2) << 1 | 
            digitalRead(BUTTON3) << 2;
            
    if (oldState != state) {
        oldState  = state;
        return true;
    }
    return false;
}

bool Buttons::buttonDown(uint8_t id) {
    return (state & (1 << id)) == 0;
}

