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
#include "unbufferedWriter.h"

UnbufferedWriter::UnbufferedWriter() {} 

void UnbufferedWriter::open(const char* filename) {
  Serial.printf("[FBUF] Open %s\n", filename);
  if (!file.open(filename, O_CREAT | O_TRUNC | O_RDWR)) {
    Serial.printf("[FBUF] Error opening %s\n", filename);  
  } else {
    Serial.printf("[FBUF] Opened %s\n", filename);
  }  
}

void UnbufferedWriter::write(uint8_t* data, size_t length) {
  size_t written = file.write(data, length);
  if (written != length) {
    Serial.printf("ERROR writing %d/%d\n", written, length);
  }
 }

void UnbufferedWriter::close() {
  bool res = file.close();
  if (!res) {
      Serial.printf("ERROR closing file");
  }
}
