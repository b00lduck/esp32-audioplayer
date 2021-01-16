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
#include "Arduino.h"
#include "config.h"
#include <MFRC522.h>

class RFID {

  public:
    enum CardState {  
       NO_CHANGE,
       FAULTY_CARD,
       NEW_CARD,
       REMOVED_CARD
    };
  
    RFID(uint8_t _csPin, uint8_t _rstPin);
    void init();
    CardState checkCardState();
    void currentCardAsString(char *buf);

    byte currentCard[CARD_ID_BYTE_ARRAY_LENGTH];    

    /**
     * Format 4-Byte array to string like "0A1B2C3D"
     */
    static void formatCardId(char cardIdAsString[CARD_ID_STRING_LENGTH], byte *buffer) {
        snprintf(cardIdAsString, CARD_ID_STRING_LENGTH, "%02X%02X%02X%02X", buffer[0], buffer[1], buffer[2], buffer[3]);
    }

  private:
    uint8_t csPin;
    uint8_t rstPin;
    MFRC522 mfrc522;

    bool cardPresent = false;    
    bool cardError = false;

    uint8_t cardFailCount = 0;    

    bool cardChanged(byte *buffer, byte bufferSize);
    void newCard(byte *buffer, byte bufferSize);   

};


