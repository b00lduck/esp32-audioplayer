/**
 * 
 * Copyright 2021 D.Zerlett <daniel@zerlett.eu>
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
#include "ndef.h"

NDEF::NDEF(MFRC522 *_mfrc522) : mfrc522(_mfrc522) { 
  rc522Utilities.setMFRC(mfrc522);
}

bool NDEF::readSector(byte* buffer, byte sector, byte block, byte numBlocks) {

  const byte trailerBlock = sector * 4 + 3;
  MFRC522::StatusCode status = mfrc522->PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &rc522Utilities.NFCKeyA, &(mfrc522->uid));
  if (status != MFRC522::STATUS_OK) {
    return false;
  }

  byte bufsize = 18;
  for (uint8_t i=0;i<numBlocks;i++) {
    mfrc522->MIFARE_Read(block+i, buffer + (i*16), &bufsize);
  }  
  return true;
}

bool NDEF::readWifiConfig(WifiConfig *wifiConfig) {
 
  byte buf[98]; 
  readSector(buf, 1, 4, 3);
  readSector(buf + 48, 2, 8, 3);
  mfrc522->PCD_Init();

  //rc522Utilities.dump_byte_array(buf, 98);

  // first four sector bytes must be 0,0,3,5c
  if (buf[0] != 0 || buf[1] != 0 || buf[2] != 3 || buf[3] != 0x5c) {
    Serial.println(F("[RFID] Non-NDEF card detected"));
    return false;
  }

  // The record header must be 0xda (1 1 0 1 1 010)  
  if (buf[4] != 0xda) {
    Serial.println(F("[RFID] Unsupported NDEF header"));
    return false;
  }

  // Check MIME type (record type)  
  char mime[] = "application/vnd.wfa.wsc";
  if (buf[5] != strlen(mime) || (strncmp((const char*)(buf+8), mime, strlen(mime)) != 0)) {
    Serial.println("[RFID] Unsupported MIME type");    
    return false;
  }
    
  // check ID length
  if (buf[7] != 1) {
    Serial.println(F("[RFID] Unsupported ID length"));    
    return false;
  }

  uint8_t payloadLen = buf[6];
  byte *payload = buf + strlen(mime) + 9;
  byte *payloadEnd = payload + payloadLen;

  // check if first TLV is credential (0x100E)
  if (payload[0] != 0x10 || payload[1] != 0x0e) {
    Serial.println(F("[RFID] Malformed MIME payload: not a credential"));    
    return false;
  }

  // advance by 4 bytes to the MIME record in the payload data
  payload += 4;

  bool passwordSet = false;
  bool ssidSet = false;

  while (payload+4 <= payloadEnd) {

    // read next chunk
    if (payload[0] != 0x10) {
      Serial.printf("Malformed MIME payload: expected chunk header not found: %02x %02x\n", payload[0], payload[1]);
      return false;;
    }

    switch(payload[1]) {

      // network name
      case 0x45: {
          uint16_t chunkLen = (payload[2]<<8) + payload[3];
          if (chunkLen > 32) {
            Serial.println(F("SSID too long"));  
            return false;
          }
          memset(wifiConfig->SSID, 0, 33);
          strncpy(wifiConfig->SSID, (const char*)(payload + 4), chunkLen);
          ssidSet = true;          
          payload += chunkLen + 4;
          break;
        }

      // network key
      case 0x27: {
          uint16_t chunkLen = (payload[2]<<8) + payload[3];
          if (chunkLen > 63) {
            Serial.println(F("Password too long"));  
            return false;
          }
          memset(wifiConfig->password, 0, 64);
          strncpy(wifiConfig->password, (const char*)(payload + 4), chunkLen);
          passwordSet = true;
          payload += chunkLen + 4;          
          break;
        }       

      // auth type
      case 0x03: {
          uint16_t chunkLen = (payload[2]<<8) + payload[3];
          if (chunkLen != 2) {
            Serial.println(F("Invalid auth type"));  
            return false;
          }
          //Serial.printf("auth type: %02x %02x\n", payload[4], payload[5]);
          payload += chunkLen + 4;
          break;
        }                 

      // encryption type
      case 0x0f: {
        uint16_t chunkLen = (payload[2]<<8) + payload[3];
        if (chunkLen != 2) {
          Serial.println(F("Invalid encryption type"));  
          return false;
        }
        //Serial.printf("encryption type: %02x %02x\n", payload[4], payload[5]);
        payload += chunkLen + 4;
        break;
      }                 

      // MAC address, ignore
      case 0x20: 
      // Vendor ext, ignore
      case 0x49: {
        uint16_t chunkLen = (payload[2]<<8) + payload[3];
        payload += chunkLen + 4;
        break;
      }

      default:
        Serial.printf("Unknown chunk: %02x\n", payload[1]);  
        return false;
    }

  }

  if (!passwordSet) {
    Serial.println(("Missing network key field"));
    return false;
  }

  if (!ssidSet) {
    Serial.println(("Missing network SSID field"));      
    return false;
  }

  return true;
}
