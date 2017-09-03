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
 
#include "oled.h"

Oled::Oled(uint8_t _i2cAddress) : 
  i2cAddress(_i2cAddress),
  ssd1306(4)
  {}

void Oled::init() {
 ssd1306.begin(SSD1306_SWITCHCAPVCC, i2cAddress, false);  
 ssd1306.clearDisplay();
 ssd1306.setTextColor(1);
 ssd1306.setTextSize(2);
 ssd1306.setCursor(34,10);
 ssd1306.println("Hello");
 ssd1306.display();
}

void Oled::clear() {
 ssd1306.clearDisplay();
 ssd1306.display();
}

void Oled::trackName(char* trackName) {
 ssd1306.fillRect(0,0,127,10,BLACK); 
 ssd1306.setTextColor(1);
 ssd1306.setTextSize(1);
 ssd1306.setCursor(0,0);
 ssd1306.printf("%s", trackName);
 ssd1306.display();
}

void Oled::cardId(byte *card, uint8_t len) {

 ssd1306.fillRect(0,21,127,40,BLACK); 
 ssd1306.setTextColor(1);
 ssd1306.setTextSize(1);
 ssd1306.setCursor(0,21);
 for (byte i = 0; i < len; i++) {
   ssd1306.print(card[i] < 0x10 ? " 0" : " ");
   ssd1306.print(card[i], HEX);
 }
 ssd1306.display();
}
