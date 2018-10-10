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

    byte currentCard[ID_BYTE_ARRAY_LENGTH];

  private:
    uint8_t csPin;
    uint8_t rstPin;
    MFRC522 mfrc522;

    bool cardPresent = false;    
    uint8_t cardFailCount = 0;    

    bool cardChanged(byte *buffer, byte bufferSize);
    void newCard(byte *buffer, byte bufferSize);   

};


