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
#include "http.h"

void HTTP::handlerCardGet(AsyncWebServerRequest *request) {  
  File it;
  Mapper::MapperError err = mapper->createMappingIterator(&it);
  if (err != OK) {
    AsyncWebServerResponse *response = request->beginResponse(500, F("text/plain"), "Could not create cards list.");    
    request->send(response);
    return;
  }
  
  AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif
  response->print("[");
  Mapper::Mapping m;
  err = mapper->nextMapping(&it, &m);
    
  while (err != Mapper::MapperError::NO_MORE_CARDS) {
        
    if (err == Mapper::MapperError::OK) {
      response->printf("{\"cardId\":\"%s\", \"name\":\"%s\", \"files\": [", m.mappingMeta.cardId, m.mappingMeta.name);         
      for(uint8_t i=0; i<m.playlist.numEntries; i++) {
        response->printf("\"%s\"", m.playlist.entries[i]);
        if (i<m.playlist.numEntries-1) {
          response->println(",");
        }
      }          
      response->print("]}");
    }

    err = mapper->nextMapping(&it, &m);
    if (err == Mapper::MapperError::OK) {
      response->println(",");
    }
    
  }
  response->print("]");
  request->send(response);
}


void HTTP::handlerCurrentCardGet(AsyncWebServerRequest *request) {  
  char buf[CARD_ID_STRING_LENGTH];
  rfid->currentCardAsString(buf);
  AsyncWebServerResponse *response = request->beginResponse(200, F("text/plain"), buf);
  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif
  request->send(response);
}