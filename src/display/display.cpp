/**
 * 
 * Copyright 2018-2020 D.Zerlett <daniel@zerlett.eu>
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
#include "display.h"
#include "config.h"

#define RED RgbColor(10,0,0)
#define YELLOW RgbColor(11,9,0)
#define GREEN RgbColor(0,10,0)
#define OFF RgbColor(0,0,0)

void renderTask(void *parameter){
  while (true) {
    Display *display = (Display*)parameter;
    display->render();
    delay(10);
  } 
}

Display::Display() : pixels(4, WS2812_DATA_PIN), mode(INITIALIZING) {}

void Display::init() {
  xTaskCreate(renderTask, "", 20000, this, 1, NULL);
}

void Display::setMode(DisplayMode _mode) {
  mode = _mode;
}

void Display::render() {

  pixels.Begin();
  switch(mode) {
    case INITIALIZING:
      pixels.ClearTo(RgbColor(7,7,0));
      break;
    case IDLE:
      pixels.ClearTo(RgbColor(0,7,0));
      break;
    case PLAYING: 
      for (int i=0; i< 4; i++) {
        uint8_t red = (sin(millis() / 70.0f + i * 300) + 1) / 2.0f * 15.0f;
        uint8_t green = (sin(millis() / 230.0f + i * 300) + 1) / 2.0f * 15.0f;
        uint8_t blue = (sin(millis() / 120.0f + i * 300) + 1) / 2.0f * 15.0f;
        pixels.SetPixelColor(i, RgbColor(red,green,blue));
      }
      break;      
    case ERROR_SD_INIT:
      pixels.SetPixelColor(0, RED);
      pixels.SetPixelColor(1, OFF);
      pixels.SetPixelColor(2, OFF);
      pixels.SetPixelColor(3, YELLOW);
      break;
    case ADMIN_INIT:
      pixels.SetPixelColor(0, YELLOW);
      pixels.SetPixelColor(1, RED);
      pixels.SetPixelColor(2, RED);
      pixels.SetPixelColor(3, YELLOW);
      break;
    case ADMIN_IDLE:
      pixels.SetPixelColor(0, YELLOW);
      pixels.SetPixelColor(1, GREEN);
      pixels.SetPixelColor(2, GREEN);
      pixels.SetPixelColor(3, YELLOW);
      break;
    default:
      pixels.ClearTo(RgbColor(7,0,0));
      break;

  }
  pixels.Show();
}


