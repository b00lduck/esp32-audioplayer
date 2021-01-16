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
#include "ringbuffer.h"

RingBuffer::RingBuffer(uint32_t s)  {
  size = s;
  buf = (uint8_t *) malloc (size);
  rindex = size - 1;
  windex = 0;
  count = 0;
}

// True is at least one byte of free space is available
bool RingBuffer::space() {
  return ( count < size );     
}

// Return number of bytes available
uint16_t RingBuffer::avail() {
  return count;                     
}

// Put one byte in the ringbuffer
void RingBuffer::put(uint8_t b) {
  *(buf + windex) = b;
  if ( ++windex == size ) {
    windex = 0;
  }
  count++;
}

uint8_t RingBuffer::get() {
  if ( ++rindex == size ) { 
    rindex = 0;                   
  }
  count--;                         
  return *(buf + rindex);
}

void RingBuffer::empty() {
  windex = 0;                
  rindex = size - 1;
  count = 0;
}
