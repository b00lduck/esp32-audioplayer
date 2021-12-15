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
#include "../display/display.h"

Player::Player(Fatal *fatal, VS1053 *vs1053, SDCard *sdCard, Display *display) : 
    fatal(fatal),     
    vs1053(vs1053),
    sdCard(sdCard),
    display(display),
    state(STOPPED), 
    oldState(INITIALIZING),
    ringBuffer(20000),
    dataFile(),
    currentVolume(VOLUME),      
    lastTime(0),
    idleTime(0) {}

void Player::init() {
  // Initialize audio decoder
  vs1053->begin();
  #ifndef FAST_BOOT
    vs1053->printDetails();
  #endif
}

void Player::play() {
  display->setMode(Display::DisplayMode::PLAYING);
  playNextFile();
}

void Player::playNextFile() { 

  char *entry = playlist.getNextEntry();
  if (entry == NULL) {
    Serial.println("Playlist exhausted.");
    stop(true);   
    return;
  }

  display->setMode(Display::DisplayMode::PLAYING);

  Serial.printf("Playing next file in playlist (%d/%d): %s\n", playlist.currentEntryIndex, playlist.numEntries, entry);
 
  enableAmp();
  
  char* filename = entry;

  dataFile = sdCard->sd.open(filename, O_READ);
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
  vs1053->setVolume(currentVolume);                 

  uint8_t tone[4] = {0,0,15,15};
  vs1053->setTone(tone);

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
  if (playlist.currentEntryIndex < playlist.numEntries) {
    stop(false); 
    playNextFile();
  } else {
    stop(true); 
  }
}

void Player::initAmp() {
  // Initialize GPIOs for "amplifier enable"
  pinMode(AMP_ENABLE_PIN, OUTPUT);
  digitalWrite(AMP_ENABLE_PIN, HIGH); 
  
  // Lower volume
  pinMode(BUTTON0_PIN, OUTPUT);
  digitalWrite(BUTTON0_PIN, HIGH); 
  delay(VOLUME_CLICKS_DELAY);
  for(uint8_t i=0;i<VOLUME_CLICKS;i++) {
    digitalWrite(BUTTON0_PIN, LOW); 
    delay(VOLUME_CLICKS_DELAY);
    digitalWrite(BUTTON0_PIN, HIGH); 
    delay(VOLUME_CLICKS_DELAY);
  }

  // Disable amp
  digitalWrite(AMP_ENABLE_PIN, LOW); 
  ampEnabled = false;
}

void Player::disableAmp() {
  digitalWrite(AMP_ENABLE_PIN, LOW);
  ampEnabled = false;
}

void Player::enableAmp() {
  if (!ampEnabled) {
    delay(200);
    digitalWrite(AMP_ENABLE_PIN, HIGH);
    ampEnabled = true;
  }
}

void Player::stop(bool _disableAmp) {  
  idleTime = 0;
  display->setMode(Display::DisplayMode::IDLE);
  if (_disableAmp) {
    disableAmp();
  }
  dataFile.close();
  vs1053->processByte(0, true);
  vs1053->setVolume(0);                  
  vs1053->stopSong();                       
  ringBuffer.empty();                            
  state = STOPPED;    
}

void Player::process(uint32_t timeGone) {

  uint32_t maxfilechunk;  

  if (oldState != state) {
    Serial.printf("Player state is now %d, was %d.\n", state, oldState);
    oldState = state;
  }  

  switch (state) {

    case INITIALIZING:
      idleTime += timeGone;
      break;      

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
      while (vs1053->data_request() && ringBuffer.avail()) { 
        vs1053->processByte(ringBuffer.get(), false);
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
  vs1053->setVolume(currentVolume);
 }

void Player::increaseVolume() {
    setVolume(currentVolume+1);
}

void Player::decreaseVolume() {
    setVolume(currentVolume-1);
}
