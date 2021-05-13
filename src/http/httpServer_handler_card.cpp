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
#include "httpServer.h"
#include "utils.h"

void HTTPServer::handlerCardPut() {  
  
  const char *cardId = server.pathArg(0).c_str();

  Serial.println(strlen(cardId));

  if (strlen(cardId) != CARD_ID_LENGTH) {
    sendError(400, "Card id length mismatch");
    return;
  }

  if (!server.hasArg("name")) {
    sendError(400, "Missing card name");
    return;
  }
 
  const char* cardName = (char*) server.arg("name").c_str();
  if (strlen(cardName) > CARD_ID_LENGTH) {
    sendError(400, "Card name too long");
    return;
  }

  Mapper::MapperError err = mapper->writeNameToMetaFile(cardId, cardName);
  if (err == Mapper::MapperError::CARD_ID_NOT_FOUND) {
    sendError(404, "Card id not found");
    return;
  }  
  Serial.println(F("[HTTP] Card name sucessfully changed"));
  server.send(204);
  return;
}

void HTTPServer::handlerCardPost() {  

  String cardId = server.pathArg(0);

  if (cardId.length() != CARD_ID_LENGTH) {
    sendError(400, "Card id length mismatch");
    return;
  }

  String cardName = server.hasArg("name") ? server.arg("name") : "untitled";
  if (cardName.length() > MAX_CARD_NAME_LENGTH) {
    sendError(400, "Card name too long");
    return;
  }

  Mapper::MapperError err = mapper->initializeCard(cardId.c_str(), cardName.c_str());  

  if (err != Mapper::MapperError::OK) {
    sendError(500, "Could not initialize card");
    return;
  }
  
  Serial.println(F("[HTTP] Card sucessfully initialized"));
  server.send(204);
}

void HTTPServer::handlerCardGet() {  
  FILETYPE it;
  Mapper::MapperError err = mapper->createMappingIterator(&it);
  if (err != Mapper::MapperError::OK) {
    sendError(500, "Could not create cards list");    
    return;
  }
   
  String json = "[";
  
  bool first = true;
  do {

    Mapper::Mapping m;
    err = mapper->nextMapping(&it, &m);

    if (err == Mapper::MapperError::OK) {

      if (!first) {
        json += ",";
      }

      json += "{\"cardId\":\"";
      json += m.mappingMeta.cardId;
      json += "\", \"name\":\"";
      json += m.mappingMeta.name;
      json += "\", \"files\": [";

      for(uint8_t i=0; i<m.playlist.numEntries; i++) {
        json += "\"";
        json += m.playlist.entries[i];
        json += "\"";
        if (i<m.playlist.numEntries-1) {
          json += ",";
        }      
      }          
      json += "]}";

      first = false;

    } 

  } while (err != Mapper::MapperError::NO_MORE_CARDS);

  json += "]";

  server.send(200, "application/json", json);
}


void HTTPServer::handlerCurrentCardGet() {  
  char cardId[CARD_ID_STRING_BUFFER_LENGTH];
  rfid->currentCardAsString(cardId);
  stringToLower(cardId);

  if (strncmp(cardId, "00000000", CARD_ID_STRING_BUFFER_LENGTH) == 0) {     
    server.send(204);
  } else {
    Mapper::MappingMeta meta;
    Mapper::MapperError err = mapper->readMetaFile(&meta, cardId);
    String json = "";
    if (err == Mapper::MapperError::OK) {

      Playlist p;
      mapper->createPlaylist(&p, cardId);      
      json += "{\"id\":\"";
      json += cardId;
      json += "\",\"state\":\"ok\",\"numEntries\":";
      json += p.numEntries;
      json += ",\"name\":\"";
      json += meta.name;
      json += "\", \"entries\":[";

      char* entry = NULL;
      uint16_t index = 0;
      do {
        entry = p.getNextEntry();
        if (entry) {
          if (index > 0) {
            json += ",";
          }
          index++;
          json += "{\"id\":\"";
          json += index;
          json += "\",\"name\":\"";
          json += (entry + CARD_DIRECTORY_PATH_LENGTH);
          json += "\",\"path\":\"";
          json += entry;
          json += "\"}";
        }
      } while(entry);         
      json += "]}";
    } else {
      json += "{\"id\":\"";
      json += cardId;
      json += "\",\"state\":\"uninitialized\"}";
    }

    server.send(200, "application/json", json);
  }

}