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

#include <stdio.h>
#include <string.h>
#include <Wire.h>
#ifdef OTA_ENABLED
  #include <ArduinoOTA.h>
#endif

#include "config.h"
#include "buttons.h"
#include "player/VS1053.h"
#include "player/player.h"
#include "storage/sd.h"
#include "storage/mapper.h"
#include "display/display.h"
#include "http/httpServer.h"

#include "rfid/ndef.h"
#include "fatal.h"

Display   display;
Fatal     fatal;

VS1053    vs1053(VS1053_XCS_PIN, VS1053_XDCS_PIN, VS1053_DREQ_PIN, VS1053_XRESET_PIN);
RFID      rfid(MFRC522_CS_PIN, MFRC522_RST_PIN);
SDCard    sd(SD_CS_PIN);

Player    player(&fatal, &vs1053, &sd, &display);

Mapper    mapper(&sd);
Buttons   buttons;
HTTPServer http(&rfid, &mapper, &sd);

void switchToPlayerMode();
void switchToAdminMode(NDEF::WifiConfig *wifiConfig);

void setup() {
 
  Serial.begin(115200);                            
  Serial.println("MP3 player V2 now Booting...");

  display.init();  

  // Set shutdown pin to input
  pinMode(SHUTDOWN_PIN, INPUT);

  player.initAmp();
  
  // Initialize buttons
  buttons.init();

  // Initialize GPIO for battery voltage
  pinMode(ADC_BATT, ANALOG); 

  #ifdef OTA_ENABLED
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
          type = "sketch";
        } else {
          // U_SPIFFS
          type = "filesystem";
        }
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("[OTA] Start updating firware");
      })
      .onEnd([]() {
        Serial.println("\n[OTA] End");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("[OTA] Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });

    ArduinoOTA.begin();  
  #endif

  SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);
  Serial.println("[ OK ] SPI init completed");

  switchToPlayerMode();
}

uint16_t lpf = 0;
uint32_t showBattTimer = 0;
uint32_t lastTime = 0;
uint32_t uptime = 0;
uint32_t uptimeSecs = 0;

enum MAIN_MODE {
  PLAYER,
  ADMIN
};

MAIN_MODE mode = PLAYER;

void switchToPlayerMode() {
  display.setMode(Display::DisplayMode::INITIALIZING);
  
  // Initialize RFID reader
  rfid.init();
  Serial.println("[ OK ] RFID init completed");

  // Initialize SD card reader
  if (!sd.init(false)) {
    display.setMode(Display::DisplayMode::ERROR_SD_INIT);
    fatal.fatal("SD card error", "init failed");
  }
  Serial.println("[ OK ] SD/MMC init completed");  

  // initialize player
  player.init();
  Serial.println("[ OK ] MP3 init completed");
  display.setMode(Display::DisplayMode::IDLE);

  mode = PLAYER;
  display.setMode(Display::DisplayMode::IDLE);

  player.playlist.addEntry("/system/startup.mp3");
  player.play();
}

void switchToAdminMode( NDEF::WifiConfig *wifiConfig) {
  display.setMode(Display::DisplayMode::ADMIN_INIT);

  player.stop(true);

  // Initialize RFID reader
  rfid.init();
  Serial.println("[ OK ] RFID init completed");  

  // Initialize SD card reader
  if (!sd.init(false)) {
    display.setMode(Display::DisplayMode::ERROR_SD_INIT);
    fatal.fatal("SD card error", "init failed");
  }
  Serial.println("[ OK ] SD/MMC init completed");

  if (http.start(wifiConfig)) {
    mode = ADMIN;
    display.setMode(Display::DisplayMode::ADMIN_IDLE);
  } else {
    switchToPlayerMode();
  }
}


void loop() {

  #ifdef OTA_ENABLED
    ArduinoOTA.handle();  
  #endif

  uint32_t timeGone = 0;
  if (lastTime > 0){
    timeGone = millis() - lastTime;
  } 
  lastTime = millis();
  uptime += timeGone;
  uptimeSecs = uptime / 1000;

  bool changed = buttons.read();

  // Cheapo one pole IIR low pass filter with unknown cutoff frequency (because sample rate is unknown).
  // This is good enough for battery monitoring though.
  //
  // lpf:
  // 0 is 0V battery voltage  
  // 1030 is approx. 4.3V battery voltage
  // in between linear (voltage divider)
  uint16_t batt = analogRead(ADC_BATT);
  uint16_t newLpf = (float)lpf * 0.90 + (float)batt * 0.1;
  lpf = newLpf;

  // ADC Reference: 4096 = 3300mV
  // Divider: 4.2V batt -> 980mV
  
  showBattTimer += timeGone;
  if (showBattTimer > 10000) {
    showBattTimer -= 10000;
    Serial.printf("[BATT] %04d %1.2fV (%d)\n", uptimeSecs, lpf/ADC_DIVISOR, lpf);
    Serial.printf("[PLYR] %04d idle time %d\n", uptimeSecs, player.idleTime);
    Serial.printf("[HEAP] %04d %d\n", uptimeSecs, ESP.getFreeHeap());
  }
  
  NDEF::WifiConfig wifiConfig;

  // Read card
  RFID::CardState cardState = rfid.checkCardState(&wifiConfig);
  
   switch(mode) {

    case PLAYER:
      if (player.idleTime > IDLE_SHUTDOWN_FADE_START) {
          if (player.idleTime > IDLE_SHUTDOWN_TIMEOUT) { //  shutdown timer
            Serial.println("[HALT] idle shutdown");
            pinMode(SHUTDOWN_PIN, OUTPUT);
            digitalWrite(SHUTDOWN_PIN, HIGH);
            sleep(30);
          }
        } 

        // Track Control
        if (changed) {
          if (buttons.buttonDown(3)) {
            player.next();
          } else if (buttons.buttonDown(1)) {
            player.previous();
          } else if (buttons.buttonDown(2)) {
            player.stop(true);
          }
        }
              
        switch(cardState) {
          case RFID::CardState::NEW_MEDIA_CARD:            
            char currentCardString[CARD_ID_STRING_BUFFER_LENGTH];
            rfid.currentCardAsString(currentCardString);
            mapper.createPlaylist(&player.playlist, currentCardString);
            player.play();
            break;

          case RFID::CardState::NEW_WIFI_CARD:
            switchToAdminMode(&wifiConfig);
            break;            

          case RFID::CardState::REMOVED_CARD:            
            //player.stop(true);
            break;

          case RFID::CardState::FAULTY_CARD:
            player.stop(true);
            break;

          case RFID::CardState::NO_CHANGE:
            break;
        }
        player.process(timeGone);
      break;

    case ADMIN:
      display.setMode(Display::DisplayMode::ADMIN_IDLE);
      if (changed) {        
        http.shutdown();
        switchToPlayerMode();      
      }
      break;

  }  
}