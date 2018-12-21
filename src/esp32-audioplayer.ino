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

#include "config.h"
#include "tools.h"
#include "buttons.h"
#include "VS1053.h"
#ifdef OLED
  #include "oled.h"
#endif
#include "rfid.h"
#include "sd.h"
#include "mapper.h"
#include "player.h"
#include "fatal.h"

VS1053          vs1053(VS1053_XCS_PIN, VS1053_XDCS_PIN, VS1053_DREQ_PIN, VS1053_XRESET_PIN);
RFID            rfid(MFRC522_CS_PIN, MFRC522_RST_PIN);
SDCard          sd(SD_CS_PIN);

#ifdef OLED
  Oled            oled(DISPLAY_ADDRESS);
  Fatal           fatal(oled);
  Player          player(fatal, oled, vs1053);
#else
  Fatal           fatal;
  Player          player(fatal, vs1053);
#endif

Mapper          mapper;
Buttons         buttons;

void setup() {

  Serial.begin(115200);                            
  Serial.println("\nStarting...");

  // Initialize GPIOs for LEDs
  // led1 green
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  
  // led2 white (player state)
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  
  // led3 red
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, LOW);

  // led4 not used
  pinMode(LED4, OUTPUT);
  digitalWrite(LED4, LOW);  

  // Initialize GPIOs for "amplifier enable"
  pinMode(AMP_ENABLE, OUTPUT);
  digitalWrite(AMP_ENABLE, LOW); 

  // Initialize buttons
  buttons.init();

  // Initialize GPIO for battery voltage
  pinMode(ADC_BATT, ANALOG);

  Serial.println("GPIO init completed.");

  // Initialize I²C bus
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  #ifdef OLED
    oled.init();
    oled.loadingBar(0);  
  #endif  
  Wire.setClock(TWI_CLOCK);
  Serial.println("I²C init completed.");

  // Initialize SPI bus
  SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);
  #ifdef OLED
    oled.loadingBar(25);
  #endif
  Serial.println("SPI init completed.");

  // Initialize RFID reader
  rfid.init();

  #ifdef OLED
    oled.loadingBar(50);
  #endif
  
  // Initialize SD card reader
  if (!sd.init()) {
      fatal.fatal("SD card error", "init failed");
  }

  Mapper::MapperError err = mapper.init(); 
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
    case Mapper::MapperError::REFERENCED_FILE_NOT_FOUND:
      fatal.fatal("Mapping error", "Referenced file not found");   
  }    

  // initialize player
  player.init();

  oled.clear();
}

uint16_t lpf = 0;


void loop() {

  bool changed = buttons.read();

  #ifdef OLED
  if (changed) {
    oled.buttons(buttons.state);
  }  
  #endif

  // Cheapo one pole IIR low pass filter with unknown cutoff frequency (because sample rate is unknown).
  // This is good enough for battery monitoring though.
  //
  // lpf:
  // 0 is 0V battery voltage  
  // 1000 is approx. 4.12V battery voltage
  // in between linear (voltage divider)
  uint16_t batt = analogRead(ADC_BATT);
  uint16_t newLpf = (float)lpf * 0.95 + (float)batt * 0.05;
  lpf = newLpf;

  // Volume Control
  if (buttons.buttonDown(0)) {
    player.increaseVolume();
  }

  if (buttons.buttonDown(2)) {
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
            #ifdef OLED
              oled.trackName("Unknown card");
            #endif
            player.stop();        
          case Mapper::MapperError::LINE_TOO_LONG:
            fatal.fatal("Mapping error", "Long line/missing newline");         
          case Mapper::MapperError::MAPPING_FILE_NOT_FOUND:
            fatal.fatal("Mapping error", "Mapping file not found");         
          case Mapper::MapperError::REFERENCED_FILE_NOT_FOUND:
            fatal.fatal("Mapping error", "Data file not found"); 
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
