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
#include "player.h"
#include "VS1053.h"
#include <SD.h>

#ifdef OLED
  Player::Player(Fatal fatal, Oled oled, VS1053 vs1053) : 
      state(STOPPED), 
      oldState(INITIALIZING),
      fatal(fatal),     
      oled(oled),
      vs1053(vs1053),
      ringBuffer(20000),
      dataFile(),
      currentVolume(85) {}
#else 
  Player::Player(Fatal fatal, VS1053 vs1053) : 
      state(STOPPED), 
      fatal(fatal),     
      vs1053(vs1053),
      ringBuffer(20000),
      dataFile(),
      currentVolume(85) {}
#endif

void Player::init() {
  // Initialize audio decoder
  vs1053.begin();
  vs1053.printDetails();
}

void Player::play(char* filename) {

  digitalWrite(AMP_ENABLE, HIGH);  // enable amplifier
  digitalWrite(LED2, HIGH);
    
  dataFile = SD.open(filename, FILE_READ);   
  if (!dataFile) {
    fatal.fatal("Error", "Unknown IO problem");
  }
  #ifdef OLED
    oled.trackName(filename);
  #endif

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

  state = PLAYING;
  vs1053.setVolume(currentVolume);                 

  uint8_t tone[4] = {0,0,15,15};
  vs1053.setTone(tone);
}

void Player::stop() {
  digitalWrite(AMP_ENABLE, LOW);  // disable amplifier
  digitalWrite(LED2, LOW);
  dataFile.close();
  vs1053.processByte(0, true);
  vs1053.setVolume(0);                  
  vs1053.stopSong();                       
  ringBuffer.empty();                            
  state = STOPPED; 
}

void Player::process() {

  uint32_t maxfilechunk;

  if (oldState != state) {
    Serial.printf("Player state is now %d, was %d.\n", state, oldState);
    oldState = state;
  }
  
       
  switch (state) {

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
      while (vs1053.data_request() && ringBuffer.avail()) { 
        vs1053.processByte(ringBuffer.get(), false);
      }

      // stop if data ends
      if ((dataFile.available() == 0) && (ringBuffer.avail() == 0)) {      
        stop();
      }
      break;

    case STOPPED:
      #ifdef OLED
        oled.trackName("");
      #endif
      break;    
    
  }
}

void Player::increaseVolume() {
    currentVolume++;
    if (currentVolume > 100) {
      currentVolume = 100;
    } else {
      oled.volumeBar(currentVolume);
      vs1053.setVolume(currentVolume);
    }    
}

void Player::decreaseVolume() {
    currentVolume--;
    if (currentVolume < 1) {
      currentVolume = 1;
    } else {
      oled.volumeBar(currentVolume);
      vs1053.setVolume(currentVolume);
    }    
}

