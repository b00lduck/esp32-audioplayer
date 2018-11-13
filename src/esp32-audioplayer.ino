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
#include "oled.h"
#include "rfid.h"
#include "sd.h"
#include "ringbuffer.h"
#include "mapper.h"

void printDirectory();

enum playerState_t {PLAYING, STOPPED};

playerState_t   playerState;
File            dataFile;
RingBuffer      ringBuffer(20000);

VS1053          vs1053player(VS1053_XCS_PIN, VS1053_XDCS_PIN, VS1053_DREQ_PIN, VS1053_XRESET_PIN);
RFID            rfid(MFRC522_CS_PIN, MFRC522_RST_PIN);
SDCard          sd(SD_CS_PIN);

Oled            oled(DISPLAY_ADDRESS);
Mapper          mapper;

Buttons         buttons;

uint8_t currentVolume = 85;

void fatal(char* title, char* message) {
  Serial.println(F("FATAL ERROR OCCURED"));
  Serial.println(title);
  Serial.println(message);
  oled.fatalErrorMessage(title, message);
}




void setup() {

  Serial.begin(115200);                            
  Serial.println("\nStarting...");

  // Initialize GPIOs
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, LOW);

  pinMode(AMP_ENABLE, OUTPUT);
  digitalWrite(AMP_ENABLE, LOW); // disable amplifier     

  // Initialize buttons
  buttons.init();

  pinMode(ADC_BATT, ANALOG);

  // Initialize I²C bus
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  oled.init();
  oled.loadingBar(0);  
  Wire.setClock(600000L);

  // Initialize SPI bus
  SPI.begin(SPI_SCK_PIN,SPI_MISO_PIN,SPI_MOSI_PIN);
  oled.loadingBar(25);

  // Initialize RFID reader
  rfid.init();

  oled.loadingBar(50);
  
  // Initialize SD card reader
  if (!sd.init()) {
      fatal("SD card error", "init failed");
  }

  oled.loadingBar(75);        


  Mapper::MapperError err = mapper.init(); 
  switch(err) {
    case Mapper::MapperError::MALFORMED_LINE_SYNTAX:
      fatal("Mapping error", "Syntax error");
    case Mapper::MapperError::MALFORMED_CARD_ID:
      fatal("Mapping error", "Illegal char in ID");
    case Mapper::MapperError::LINE_TOO_SHORT:
      fatal("Mapping error", "Line too short");    
    case Mapper::MapperError::LINE_TOO_LONG:
      fatal("Mapping error", "Line too long");
    case Mapper::MapperError::MAPPING_FILE_NOT_FOUND:
      fatal("Mapping error", "Mapping file not found");      
    case Mapper::MapperError::MALFORMED_FILE_NAME:
      fatal("Mapping error", "Malformed file name");   
    case Mapper::MapperError::REFERENCED_FILE_NOT_FOUND:
      fatal("Mapping error", "Referenced file not found");   
  }
  

  // Initialize audio decoder
  vs1053player.begin();

  vs1053player.printDetails();
 
  playerState = STOPPED;

  oled.loadingBar(100);
  
  oled.clear();
}

void play(byte cardId[ID_BYTE_ARRAY_LENGTH]) {

  digitalWrite(AMP_ENABLE, HIGH);  // enable amplifier
   
  char filename[MAX_FILENAME_STRING_LENGTH];
  Mapper::MapperError err = mapper.resolveIdToFilename(cardId, filename);    
  switch(err) {
    case Mapper::MapperError::ID_NOT_FOUND:
      Serial.println(F("Card not found in mapping"));
      oled.trackName("Unknown card");
      stop();        
      return;
    case Mapper::MapperError::LINE_TOO_LONG:
      fatal("Mapping error", "Long line/missing newline");         
      break;
    case Mapper::MapperError::MAPPING_FILE_NOT_FOUND:
      fatal("Mapping error", "Mapping file not found");         
      break;
    case Mapper::MapperError::REFERENCED_FILE_NOT_FOUND:
      fatal("Mapping error", "Data file not found"); 
      break;    
  } 
  
  dataFile = SD.open(filename, FILE_READ);   
  if (!dataFile) {
    fatal("Error", "Unknown IO problem");
  }
  oled.trackName(filename);

  // skip ID3v2 tag if present
  uint8_t header[10];
  uint16_t n = dataFile.read(header, 10);
  if ((header[0] == 'I') && (header[1] == 'D') && (header[2] == '3')) {    
    uint32_t header_size = header[9] + ((uint16_t)header[8] << 7) + ((uint32_t)header[7] << 14) + ((uint32_t)header[6] << 21);
    Serial.printf("Found ID3v2 tag at beginning, skipping %d bytes\n", header_size);
    dataFile.seek(header_size);    
  } else {
    dataFile.seek(0);
  }  

  delay(300);

  playerState = PLAYING;
  vs1053player.setVolume(currentVolume);                 

  uint8_t tone[4] = {0,0,15,15};
  vs1053player.setTone(tone);
}

void stop() {
  digitalWrite(AMP_ENABLE, LOW);  // disable amplifier
  dataFile.close();
  vs1053player.processByte(0, true);
  vs1053player.setVolume(0);                  
  vs1053player.stopSong();                       
  ringBuffer.empty();                            
  playerState = STOPPED; 
}

uint16_t lpf = 0;


void loop() {

  bool changed = buttons.read();

  if (changed) {
    oled.buttons(buttons.state);
  }  

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
    currentVolume++;
    if (currentVolume > 100) currentVolume = 100;
    vs1053player.setVolume(currentVolume);
  }
  if (buttons.buttonDown(1)) {
    currentVolume--;
    if (currentVolume < 1) currentVolume = 1;
    vs1053player.setVolume(currentVolume);
  }

  RFID::CardState cardState = rfid.checkCardState();
  
  switch(cardState) {
    case RFID::CardState::NEW_CARD:
      play(rfid.currentCard);
      break;
    case RFID::CardState::REMOVED_CARD:
      Serial.println("removed card");
      stop();
      break;
    case RFID::CardState::FAULTY_CARD:
      Serial.println("faulty card");
      stop();
      break;
    case RFID::CardState::NO_CHANGE:
      break;
  }

  uint32_t maxfilechunk;
    
  switch (playerState) {

    case PLAYING:      
      // fill ring buffer with MP3 data
      maxfilechunk = dataFile.available();
      if (maxfilechunk > 1024) {
        maxfilechunk = 1024;      
      }
      while (ringBuffer.space() && maxfilechunk--) {
        ringBuffer.put(dataFile.read());
      } 
      
      // Try to keep VS1053 filled
      while (vs1053player.data_request() && ringBuffer.avail()) { 
        vs1053player.processByte(ringBuffer.get(), false);
      }

      // stop if data ends
      if ((dataFile.available() == 0) && (ringBuffer.avail() == 0)) {      
        stop();
      }
      break;

    case STOPPED:
      oled.trackName("");
      break;    
    
  }

}
