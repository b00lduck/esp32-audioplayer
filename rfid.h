/**
 * 
 * Copyright 2017 D.Zerlett <daniel@zerlett.eu>
 * 
 * This file is part of esp8266-audioplayer.
 * 
 * esp8266-audioplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * esp8266-audioplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esp8266-audioplayer. If not, see <http://www.gnu.org/licenses/>.
 *  
 */
#include "Arduino.h"
#include "config.h"
#include "csmux.h"
#include <MFRC522.h>

class RFID {

  public:
    enum CardState {  
       NO_CHANGE,
       FAULTY_CARD,
       NEW_CARD,
       REMOVED_CARD
    };
  
    RFID(CSMultiplexer *_csMux, uint8_t _csAddress, uint8_t _rstAddress);
    void init();
    CardState checkCardState();

    byte currentCard[ID_BYTE_ARRAY_LENGTH];

  private:
    CSMultiplexer *csMux;
    uint8_t csAddress;
    uint8_t rstAddress;
    MFRC522 mfrc522;

    bool cardPresent = false;    
    uint8_t cardFailCount = 0;    

    bool cardChanged(byte *buffer, byte bufferSize);
    void newCard(byte *buffer, byte bufferSize);   

};


