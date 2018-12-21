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

#ifdef OLED
#include "oled.h"

Oled::Oled(uint8_t _i2cAddress) : 
  i2cAddress(_i2cAddress),
  ssd1306(-1)
  {}

void Oled::init() {
  ssd1306.begin(SSD1306_SWITCHCAPVCC, i2cAddress, false);  
  ssd1306.clearDisplay();
  ssd1306.fillRect(0,0,127,40,WHITE);
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

/**
 * Display an error message and loop forever
 */
void Oled::fatalErrorMessage(char* error, char* info) {
  Serial.printf("Fatal: %s - %s\n", error, info);  
  ssd1306.setTextColor(1);
  ssd1306.setTextSize(1);
  boolean showBox = true;
  while (1) {
    ssd1306.clearDisplay();
    if (showBox) {
      ssd1306.fillRect(0,0,127,13,1);
      ssd1306.fillRect(3,2,121,9,0);
    }
    ssd1306.setCursor(19,3);
    ssd1306.printf("GURU MEDITATION");
    ssd1306.setCursor(0,15);
    ssd1306.printf("%s", error);
    ssd1306.setCursor(0,25);
    ssd1306.printf("%s", info);   
    ssd1306.display();  
    delay(500);
    showBox ^= 1;
  }
}

/**
 * Display a loading bar
 */
void Oled::loadingBar(uint8_t percent) {
   ssd1306.clearDisplay();
   ssd1306.fillRect(9,10,108,13,1);
   ssd1306.fillRect(12,12,102,9,0);
   ssd1306.fillRect(13,13,percent,7,1);
   ssd1306.display();  
}

/**
 * Display a volume bar
 */
void Oled::volumeBar(uint8_t percent) {
  loadingBar(percent);
}

void Oled::buttons(char buttons) {
  for(int i=0; i<4; i++) {    
    ssd1306.fillRect(i * 5, 25, 4, 4, 1);
    if (buttons & 1 << i) {
      ssd1306.fillRect(i * 5 + 1, 26, 2, 2, 0);
    }
  }
  ssd1306.display();
}
#endif