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
#include "oled.h"
#include "VS1053.h"
#include "ringbuffer.h"


enum playerState_t {PLAYING, STOPPED};

class Player {

  private:
    playerState_t state;
    Fatal fatal;
    Oled oled;
    VS1053 vs1053;
    RingBuffer ringBuffer;

    File dataFile;

    uint8_t currentVolume;

  public:
    Player(Fatal fatal, Oled oled, VS1053 vs1053);
    void init();
    void play(char* filename);
    void stop();
    void process();

    void increaseVolume();
    void decreaseVolume();
};