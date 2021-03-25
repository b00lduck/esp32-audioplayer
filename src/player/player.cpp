/**
 * 
 * Copyright 2018-2021 D.Zerlett <daniel@zerlett.eu>
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

Player::Player(Fatal fatal, VS1053 vs1053) : 
    state(STOPPED), 
    oldState(INITIALIZING),
    fatal(fatal),     
    vs1053(vs1053),
    ringBuffer(20000),
    dataFile(),
    currentVolume(80),      
    lastTime(0),
    idleTime(0) {}

void Player::init() {
  // Initialize audio decoder
  vs1053.begin();
  #ifndef FAST_BOOT
    vs1053.printDetails();
  #endif
}

void Player::play() {
  playNextFile();
}

void Player::playNextFile() { 

  char *entry = playlist.getNextEntry();
  if (entry == NULL) {
    Serial.println("Playlist exhausted.");
    stop(true);   
    return;
  }

  Serial.printf("Playing next file in playlist (%d/%d): %s\n", playlist.currentEntryIndex, playlist.numEntries, entry);
 
  digitalWrite(AMP_ENABLE_PIN, HIGH);  // enable amplifier
  
  char* filename = entry;

  dataFile = SD.open(filename, FILE_READ);
  if (!dataFile) {
    Serial.printf("Error opening file, skipping.");
    next();
  }

  // skip ID3v2 tag if present
  uint8_t header[10];
  dataFile.read(header, 10);
  if ((header[0] == 'I') && (header[1] == 'D') && (header[2] == '3')) {    
    uint32_t header_size = header[9] + ((uint16_t)header[8] << 7) + ((uint32_t)header[7] << 14) + ((uint32_t)header[6] << 21);
    Serial.printf("Found ID3v2 tag at beginning, skipping %d bytes\n", header_size);
    dataFile.seek(header_size);    
  } else {
    dataFile.seek(0);
  }  

  delay(100);

  trackElapsed = 0;
  state = PLAYING;
  vs1053.setVolume(currentVolume);                 

  uint8_t tone[4] = {0,0,15,15};
  vs1053.setTone(tone);

}

void Player::previous() {
  stop(false);
  if ((playlist.currentEntryIndex > 1) && (trackElapsed < 2000)){
    playlist.currentEntryIndex-=2;
  } else if (playlist.currentEntryIndex > 0) {
    playlist.currentEntryIndex--;
  }  
  playNextFile();
}

void Player::next() {
  stop(false);
  playNextFile();
}

void Player::stop(bool disableAmp) {  
  if (disableAmp) {
    digitalWrite(AMP_ENABLE_PIN, LOW);  // disable amplifier
  }
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
        
  uint32_t timeGone = 0;
  if (lastTime > 0){
    timeGone = millis() - lastTime;
  } 
  lastTime = millis();
  

  switch (state) {

    case PLAYING:      
      trackElapsed += timeGone;
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
        next();
      }
      idleTime = 0;
      break;

    case STOPPED:
      idleTime += timeGone;
      break;    
    
  }
}

void Player::setVolume(uint8_t volume) {
  currentVolume = volume;
  if (currentVolume > 100) {
    currentVolume = 100;
  }
  if (currentVolume < 1) {
    currentVolume = 1;
  }
  vs1053.setVolume(currentVolume);
 }

void Player::increaseVolume() {
    setVolume(currentVolume+1);
}

void Player::decreaseVolume() {
    setVolume(currentVolume-1);
}
