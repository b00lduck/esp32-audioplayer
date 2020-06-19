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
#pragma once
#include <FS.h>
#include "Arduino.h"
#include "fatal.h"
#ifdef OLED
  #include "oled.h"
#endif
#include "VS1053.h"
#include "ringbuffer.h"

enum playerState_t {INITIALIZING, PLAYING, STOPPED};

class Player {

  private:
    playerState_t state;
    playerState_t oldState;
    Fatal fatal;
    #ifdef OLED
      Oled oled;
    #endif
    VS1053 vs1053;
    RingBuffer ringBuffer;

    File dataFile;

    uint8_t currentVolume;

    char *playlist[MAX_PLAYLIST_LENGTH];
    uint8_t playlistLen;
    uint8_t playlistIndex;
    void playNextFile();
    void clearPlaylist();
    void addPlaylistEntry(char* filename);
    void setVolume(uint8_t volume);

    uint32_t lastTime;

  public:
    #ifdef OLED
      Player(Fatal fatal, Oled oled, VS1053 vs1053);
    #else
      Player(Fatal fatal, VS1053 vs1053);
    #endif
    void init();
    void play(char* filename);
    void stop();
    void process();
    void next();

    void increaseVolume();
    void decreaseVolume();

    uint32_t idleTime;
};