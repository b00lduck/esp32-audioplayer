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
#include "Arduino.h"
#include "config.h"
#include "../storage/sd.h"

#define WRITE_BUFFER_SIZE 4096*4

class BufferedWriter {

  public: 
    BufferedWriter(SDCard sdCard);
    void write(uint8_t* data, size_t length);
    void open(const char* filename);
    void flush();

  private:
    size_t flushCount = 0;
    FILETYPE file;
    char filename[255];
    uint8_t *writeBuffer;
    size_t writeBufferPos = 0;
    size_t currentFileWritePos = 0;
    SDCard sdCard;

};
