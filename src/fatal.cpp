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
#include "fatal.h"

#ifdef OLED
  Fatal::Fatal(Oled oled) : oled(oled) {}
#else
  Fatal::Fatal() {}
#endif

void Fatal::fatal(char* title, char* message) {
  Serial.println(F("FATAL ERROR OCCURED"));
  Serial.println(title);
  Serial.println(message);
  #ifdef OLED
    oled.fatalErrorMessage(title, message);
  #endif
}

