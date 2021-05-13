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
#include "sd.h"
#include "config.h"
#include "sdios.h"
#include "SdFat.h"

#define SD_NORMAL_CONFIG SdSpiConfig(csPin, SHARED_SPI, SPI_SPEED_PLAYER)
#define SD_TURBO_CONFIG SdSpiConfig(csPin, DEDICATED_SPI, SPI_SPEED_ADMIN)

SDCard::SDCard(uint8_t _csPin) : csPin(_csPin) {}

bool SDCard::init(bool turbo) {

  if (!sd.begin(turbo ? SD_TURBO_CONFIG : SD_NORMAL_CONFIG)) {
    sd.initErrorHalt(&Serial);
  }

  if (turbo) {
    Serial.println(F("[SD-Card] media initialized with TURBO"));
  } else {
    Serial.println(F("[SD-Card] media initialized (slow)"));
  }
  
  assureDirectory("/cards");
  assureDirectory("/system");
  return true;
}

void SDCard::format() {
    Serial.println("Formatting in 10 secs...");
    sleep(10);
    SdCardFactory cardFactory;
    SdCard* m_card = cardFactory.newCard(SD_NORMAL_CONFIG);
    FatFormatter fatFormatter;
    uint8_t sectorBuffer[512];
    fatFormatter.format(m_card, sectorBuffer, &Serial);
    Serial.println("done");
}

bool SDCard::assureDirectory(const char path[]) {
    Serial.printf("[SD-Card] Assuring directory %s\n", path);        
    if (!sd.exists(path)) {
        return sd.mkdir(path);
    }
    FILETYPE chk = sd.open(path);
    if (!chk.isDirectory()) {
        Serial.printf("[SD-Card] There is a file named %s on the SD card. Please remove this file from the card.", path);
        return false;
    }
    return true;
}
