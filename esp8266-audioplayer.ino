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

#include <stdio.h>
#include <string.h>
#include <SdFat.h>
#include <MFRC522.h>
#include <Wire.h>

#include "tools.h"
#include "VS1053.h"
#include "oled.h"
#include "ringbuffer.h"
#include "mapper.h"

extern "C" {
  #include "user_interface.h"
}

// GPIO for VS1053 
#define VS1053_XCS_ADDRESS     0
#define VS1053_XDCS_ADDRESS    1
#define VS1053_DREQ           16
#define VS1053_XRESET_ADDRESS  5 

// GPIO for SD card
#define SD_CS_ADDRESS          2

// GPIO for MFRC522
#define MFRC522_CS_ADDRESS     3
#define MFRC522_RST_ADDRESS    4

SdFat sd;

void printDirectory();

enum playerState_t {PLAYREQ, PLAYING, STOPREQ, STOPPED};

playerState_t   playerState;
File            dataFile;
CSMultiplexer   csMux(0x20);
RingBuffer      ringBuffer(20000);
VS1053          vs1053player(&csMux, VS1053_XCS_ADDRESS, VS1053_XDCS_ADDRESS, VS1053_DREQ, VS1053_XRESET_ADDRESS);
MFRC522         mfrc522(NULL, NULL);
Oled            oled(0x3c);
Mapper          mapper;

void fatal(char* title, char* message) {
  Serial.println("FATAL ERROR OCCURED");
  Serial.println(title);
  Serial.println(message);
  oled.fatalErrorMessage(title, message);
}

void setup() {

  Serial.begin (115200);                            
  Serial.println();
  system_update_cpu_freq(160);

  Wire.begin(2, 4);
  csMux.init();
  oled.init();
  oled.loadingBar(0);  
  Wire.setClock(600000L);

  SPI.begin();
  oled.loadingBar(25);

  if (sd.begin(
        [](){
          csMux.chipSelect(SD_CS_ADDRESS);
        },
        [](){
          csMux.chipDeselect();
        }, 
        SD_SCK_MHZ(50))){
     Serial.println("SD Card initialized.");
     printDirectory();
  } else {
     fatal("SD card error", "init failed");
  }    
  oled.loadingBar(50);
  
  mfrc522.PCD_Init(
        [](bool state){
          if (state) {
            csMux.chipSelect(MFRC522_CS_ADDRESS);  
          } else {
            csMux.chipDeselect();  
          }          
        },
        [](bool state){
          if (state) {
            csMux.chipSelect(MFRC522_RST_ADDRESS);  
          } else {
            csMux.chipDeselect();  
          }
        });
  oled.loadingBar(75);        

  mapper.init();

  vs1053player.begin();  
  vs1053player.printDetails();  

  playerState = STOPPED;

  oled.loadingBar(100);
  
  oled.clear();
}

bool cardPresent = false;
byte currentCard[32];
uint8_t cardFailCount = 0;

bool cardChanged(byte *buffer, byte bufferSize) {
  for (uint8_t i = 0; i < bufferSize; i++) {
    if (buffer[i] != currentCard[i]) {
      return true;
    }
  }
  return false;
}


/**
 * a new card was detected
 */
void newCard(byte *buffer, byte bufferSize) {
  cardPresent = true;
  if (bufferSize > sizeof(currentCard)) {
    bufferSize = sizeof(currentCard);
  }
  memcpy(currentCard, buffer, bufferSize);
  Serial.print(F("New Card with UID"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  oled.cardId(mfrc522.uid.uidByte, mfrc522.uid.size); 
  Serial.println(" detected.");  
}

void playRequest() {
  char filename[MAX_FILENAME_STRING_LENGTH];
  Mapper::ResolveError err = mapper.resolveIdToFilename(mfrc522.uid.uidByte, filename);    
  switch(err) {
    case Mapper::ResolveError::ID_NOT_FOUND:
      Serial.println("Card not found in mapping");
      oled.trackName("Unknown card");
      playerState = STOPREQ;         
      return;
    case Mapper::ResolveError::LINE_TOO_LONG:
      fatal("Mapping error", "Long line/missing newline");         
      break;
    case Mapper::ResolveError::MAPPING_FILE_NOT_FOUND:
      fatal("Mapping error", "Mapping file not found");         
      break;
    case Mapper::ResolveError::REFERENCED_FILE_NOT_FOUND:
      fatal("Mapping error", "Data file not found"); 
      break;    
  } 
  
  dataFile.open(filename, FILE_READ);   
  if (!dataFile) {
    fatal("Error", "Unknown IO problem");
  }
  oled.trackName(filename);
  playerState = PLAYING;
  vs1053player.setVolume(80);                 
}

void stopRequest() {
  dataFile.close();
  vs1053player.processByte(0, true);
  vs1053player.setVolume(0);                  
  vs1053player.stopSong();                       
  ringBuffer.empty();                            
  playerState = STOPPED; 
}

void loop() {
   
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      cardFailCount = 0;    
      if (cardChanged(mfrc522.uid.uidByte, mfrc522.uid.size)) {
        newCard(mfrc522.uid.uidByte, mfrc522.uid.size);
        playerState = PLAYREQ;      
      }      
    } else {
        Serial.print(F("Error reading card"));
        cardPresent = false;   
        playerState = STOPREQ;                    
    }
  } else {
    if (cardPresent) {
      cardFailCount++;
      if (cardFailCount > 1) {
          Serial.println(F("Card removed"));
          oled.cardId(NULL, 0);
          cardPresent = false;   
          memset(currentCard, 0, sizeof(currentCard));
          playerState = STOPREQ;                    
      }     
    }
  }

  uint32_t maxfilechunk;
    
  switch (playerState) {

    case PLAYREQ:
      playRequest();
      break;

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
        playerState = STOPREQ;
      }
      break;

    case STOPREQ:
      stopRequest();
      break;

    case STOPPED:
      oled.trackName("");
      break;    
    
  }

}

void printDirectory() {

  SdFile file;
  SdFile dirFile;

  if (!dirFile.open("/", O_READ)) {
    Serial.println("open root failed");
    return;
  }
  
  while (file.openNext(&dirFile, O_READ)) {
    if (!file.isSubDir() && !file.isHidden()) {
      file.printName(&Serial);
      Serial.println();
    }
    file.close();
  }

  dirFile.close();
}



