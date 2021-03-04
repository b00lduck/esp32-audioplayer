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


void HTTP::handlerCardPost(AsyncWebServerRequest *request) {  
  const char *cardId = request->pathArg(0).c_str();
  Serial.printf("initialize CARD ID %s", cardId);
}

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
  stringToLower(cardId);

  AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif

  if (strncmp(cardId, "00000000", CARD_ID_STRING_LENGTH) == 0) {     
      response->setCode(204);
  } else {
    Mapper::MappingMeta meta;
    Mapper::MapperError err = mapper->readMetaFile(&meta, cardId);
    if (err == Mapper::MapperError::OK) {
      Playlist p;
      mapper->createPlaylist(&p, cardId);  
      response->printf("{\"id\":\"%s\",\"state\":\"ok\",\"numEntries\":%d,\"name\":\"%s\", \"entries\":[", cardId, p.numEntries, meta.name);

      char* entry = NULL;
      bool first = true;
      do {
        entry = p.getNextEntry();
        if (entry) {
          if (!first) {
            response->print(",");            
          }
          first = false;
          response->printf("\"%s\"", entry + CARD_DIRECTORY_PATH_LENGTH);       
        }
      } while(entry);
          
      response->print("]}");

    } else {
      response->printf("{\"id\":\"%s\",\"state\":\"uninitialized\"}", cardId);
    }

  }

  request->send(response);  

}