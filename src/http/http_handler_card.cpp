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
#include "utils.h"

void HTTP::handlerCardGet(AsyncWebServerRequest *request) {  
  File it;
  Mapper::MapperError err = mapper->createMappingIterator(&it);
  if (err != Mapper::MapperError::OK) {
    AsyncWebServerResponse *response = request->beginResponse(500, F("text/plain"), "Could not create cards list.");    
    request->send(response);
    return;
  }
  
  AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif
  response->print("[");


  bool first = true;
  do {

    Mapper::Mapping m;
    err = mapper->nextMapping(&it, &m);

    if (err == Mapper::MapperError::OK) {

      if (first) {
        first = false;      
      } else {
        response->println(",");
      }

      response->printf("{\"cardId\":\"%s\", \"name\":\"%s\", \"files\": [", m.mappingMeta.cardId, m.mappingMeta.name);         
      for(uint8_t i=0; i<m.playlist.numEntries; i++) {
        response->printf("\"%s\"", m.playlist.entries[i]);
        if (i<m.playlist.numEntries-1) {
          response->println(",");
        }
      }          
      response->print("]}");

    } 

  } while (err != Mapper::MapperError::NO_MORE_CARDS);

  response->print("]");
  request->send(response);
}


void HTTP::handlerCurrentCardGet(AsyncWebServerRequest *request) {  
  char cardId[CARD_ID_STRING_LENGTH];
  rfid->currentCardAsString(cardId);

  Serial.println("reading meta file");

  stringToLower(cardId);
  
  Mapper::MappingMeta meta;
  Mapper::MapperError err = mapper->readMetaFile(cardId, &meta);

  if (err != Mapper::MapperError::OK) {
    request->send(request->beginResponse(500, F("text/plain"), "Could not read meta file"));
    return;
  }
  
  AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif

  response->printf("{\"id\":\"%s\",\"status\":\"empty\",\"name\":\"%s\"}", cardId, meta.name);

  request->send(response);

}