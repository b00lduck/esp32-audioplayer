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

#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <NeoPixelBus.h>

#include "config.h"
#include "tools.h"
#include "buttons.h"
#include "VS1053.h"
#include "rfid.h"
#include "sd.h"
#include "mapper.h"
#include "player.h"
#include "fatal.h"

VS1053          vs1053(VS1053_XCS_PIN, VS1053_XDCS_PIN, VS1053_DREQ_PIN, VS1053_XRESET_PIN);
RFID            rfid(MFRC522_CS_PIN, MFRC522_RST_PIN);
SDCard          sd(SD_CS_PIN);

Fatal           fatal;
Player          player(fatal, vs1053);

Mapper          mapper;
Buttons         buttons;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels(4, WS2812_DATA_PIN);

void setup() {

  pixels.Begin();
  pixels.SetPixelColor(0, RgbColor(20,0,0));
  pixels.SetPixelColor(1, RgbColor(0,20,0));
  pixels.SetPixelColor(2, RgbColor(0,20,0));
  pixels.SetPixelColor(3, RgbColor(20,0,0));  
  pixels.Show();

  // Set shutdown pin to high to prevent false power down
  pinMode(SHUTDOWN_PIN, OUTPUT);
  digitalWrite(SHUTDOWN_PIN, HIGH); 

  // Initialize GPIOs for "amplifier enable" and shut it down
  pinMode(AMP_ENABLE_PIN, OUTPUT);
  digitalWrite(AMP_ENABLE_PIN, LOW); 

  // Initialize buttons
  buttons.init();

  // Initialize GPIO for battery voltage
  pinMode(ADC_BATT, ANALOG);

  Serial.begin(115200);                            
  Serial.println("MP3 player V2 now Booting...");

  // Initialize SPI bus
  SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);
  Serial.println("[ OK ] SPI init completed");

  // Initialize RFID reader
  rfid.init();
  Serial.println("[ OK ] RFID init completed");
 
  // Initialize SD card reader
  if (!sd.init()) {
      fatal.fatal("SD card error", "init failed");
  }
  Serial.println("[ OK ] SD/MMC init completed");

  Mapper::MapperError err = mapper.init(); 
  if (err != Mapper::MapperError::OK) {
    //player.play("/error.mp3");
    switch(err) {
      case Mapper::MapperError::MALFORMED_LINE_SYNTAX:
        fatal.fatal("Mapping error", "Syntax error");
      case Mapper::MapperError::MALFORMED_CARD_ID:
        fatal.fatal("Mapping error", "Illegal char in ID");
      case Mapper::MapperError::LINE_TOO_SHORT:
        fatal.fatal("Mapping error", "Line too short");    
      case Mapper::MapperError::LINE_TOO_LONG:
        fatal.fatal("Mapping error", "Line too long");
      case Mapper::MapperError::MAPPING_FILE_NOT_FOUND:
        fatal.fatal("Mapping error", "Mapping file not found");      
      case Mapper::MapperError::MALFORMED_FILE_NAME:
        fatal.fatal("Mapping error", "Malformed file name");   
      #ifdef FAIL_ON_FILE_NOT_FOUND      
        case Mapper::MapperError::REFERENCED_FILE_NOT_FOUND:
          fatal.fatal("Mapping error", "Referenced file not found");   
      #endif      
    }    
  }

  pixels.SetPixelColor(0, RgbColor(0,0,0));
  pixels.SetPixelColor(1, RgbColor(0,20,0));
  pixels.SetPixelColor(2, RgbColor(0,20,0));
  pixels.SetPixelColor(3, RgbColor(0,0,0));
  pixels.Show();   

  // initialize player
  player.init();
  Serial.println("[ OK ] MP3 init completed");

  //player.play("/startup.mp3");
}

uint16_t lpf = 0;
uint16_t showBatt = 5000;

void loop() {

  bool changed = buttons.read();

  // Cheapo one pole IIR low pass filter with unknown cutoff frequency (because sample rate is unknown).
  // This is good enough for battery monitoring though.
  //
  // lpf:
  // 0 is 0V battery voltage  
  // 1000 is approx. 4.3V battery voltage
  // in between linear (voltage divider)
  uint16_t batt = analogRead(ADC_BATT);
  uint16_t newLpf = (float)lpf * 0.90 + (float)batt * 0.1;
  lpf = newLpf;

  // ADC Reference: 4096 = 3300mV
  // Divider: 4.2V batt -> 2100mV

  showBatt++;
  if (showBatt > 500) {
    Serial.printf("[BATT] %1.2fV (%d)\n", lpf/ADC_DIVISOR, lpf);
    showBatt = 0;

    Serial.printf("[PLYR] idle time %d\n", player.idleTime);
  }
  
  if (player.idleTime > 28860044) { // approx 15 min shutdown timer
    Serial.println("[HALT] idle shutdown");
    pixels.SetPixelColor(0, RgbColor(0,0,0));
    pixels.SetPixelColor(1, RgbColor(0,0,0));
    pixels.SetPixelColor(2, RgbColor(0,0,0));
    pixels.SetPixelColor(3, RgbColor(0,0,0));
    pixels.Show();
    sleep(1);
    digitalWrite(SHUTDOWN_PIN, LOW);
    sleep(30);
  }
  
  // Volume Control
  if (buttons.buttonDown(4)) {
    player.increaseVolume();
  }

  if (buttons.buttonDown(0)) {
    player.decreaseVolume();
  }


  RFID::CardState cardState = rfid.checkCardState();
  
  switch(cardState) {
    case RFID::CardState::NEW_CARD:
      {
        char filename[MAX_FILENAME_STRING_LENGTH];
        Mapper::MapperError err = mapper.resolveIdToFilename(rfid.currentCard, filename);    
        switch(err) {
          case Mapper::MapperError::ID_NOT_FOUND:
            Serial.println(F("Card not found in mapping"));
            player.stop();        
          case Mapper::MapperError::LINE_TOO_LONG:
            fatal.fatal("Mapping error", "Long line/missing newline");         
          case Mapper::MapperError::MAPPING_FILE_NOT_FOUND:
            fatal.fatal("Mapping error", "Mapping file not found");         
          #ifdef FAIL_ON_FILE_NOT_FOUND  
            case Mapper::MapperError::REFERENCED_FILE_NOT_FOUND:
              fatal.fatal("Mapping error", "Data file not found"); 
          #endif              
          case OK:
            player.play(filename);
        } 
      }
      break;
    case RFID::CardState::REMOVED_CARD:
      Serial.println("removed card");
      player.stop();
      break;
    case RFID::CardState::FAULTY_CARD:
      Serial.println("faulty card");
      player.stop();
      break;
    case RFID::CardState::NO_CHANGE:
      break;
  }

  player.process();
}
