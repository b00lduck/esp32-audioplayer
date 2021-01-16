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
#pragma once
#include <FS.h>
#include "Arduino.h"
#include "fatal.h"
#include "VS1053.h"
#include "ringbuffer.h"
#include "../storage/mapper.h"

enum playerState_t {INITIALIZING, PLAYING, STOPPED};

class Player {

  private:
    playerState_t state;
    playerState_t oldState;
    Fatal fatal;
    VS1053 vs1053;
    RingBuffer ringBuffer;

    File dataFile;

    Playlist *playlist;

    uint8_t currentVolume;

    void playNextFile();    
    

    void setVolume(uint8_t volume);

    uint32_t lastTime;

  public:
    Player(Fatal fatal, VS1053 vs1053);
    void init();
    void play(Playlist *playlist);
    void stop();
    void process();
    void next();

    void increaseVolume();
    void decreaseVolume();

    uint32_t idleTime;
};