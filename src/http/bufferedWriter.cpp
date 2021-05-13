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
#include "bufferedWriter.h"

BufferedWriter::BufferedWriter(SDCard sdCard) : sdCard(sdCard) { 
  writeBuffer = (uint8_t*) malloc(WRITE_BUFFER_SIZE);  
} 

void BufferedWriter::open(const char* filename) {
  Serial.printf("[FBUF] Open %s\n", filename);
  if (!file.open(filename, O_CREAT | O_TRUNC | O_RDWR)) {
    Serial.printf("[FBUF] Error open %s\n", filename);  
  } else {
    Serial.printf("[FBUF] Opened %s\n", filename);
  }  
  file.seek(0);
  currentFileWritePos = 0;
}

void BufferedWriter::write(uint8_t* data, size_t length) {

  //Serial.printf("[FBUF] Write %d\n", length);
  
  if (writeBufferPos + length > WRITE_BUFFER_SIZE) {

    // fill up to end of buffer
    size_t amountLeft = WRITE_BUFFER_SIZE - writeBufferPos;
    memcpy(writeBuffer + writeBufferPos, data, amountLeft);
    
    writeBufferPos += amountLeft;    

    // write buffer to file
    flush();

    // copy rest of data to buffer
    memcpy(writeBuffer, data+amountLeft, length-amountLeft);
    writeBufferPos += length-amountLeft;

  } else {
    // just fill up buffer
    memcpy(writeBuffer + writeBufferPos, data, length);
    writeBufferPos += length;
  }
 }

void BufferedWriter::flush() {       
  file.seekSet(currentFileWritePos);    
  size_t written = file.write(writeBuffer, writeBufferPos);      
  while (written != writeBufferPos) {      
    Serial.printf("RETRY %d/%d\n", written, writeBufferPos);
    delay(2000);     
    written = file.write(writeBuffer, writeBufferPos);            
  } 
  currentFileWritePos += writeBufferPos;
  Serial.printf(".");
  
  writeBufferPos = 0;
  file.flush();
}
