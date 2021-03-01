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

void HTTP::handlerFileGet(AsyncWebServerRequest *request) {  

  File it;
  const char *path = request->pathArg(0).c_str();
  
  if (strlen(path) == 0) {
      path = "/";      
  } 

  Mapper::MapperError err = mapper->createFileIterator(&it, path);
    
  switch(err) {
      case Mapper::MapperError::DIRECTORY_NOT_FOUND:
        request->send(request->beginResponse(404, F("text/plain"), "File not found"));
        return;
      case OK:
        break;
      default:
        request->send(request->beginResponse(500, F("text/plain"), "Could not create file list"));
        return;     
  }
  
  AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif
  response->print("[");
  char name[MAX_FILENAME_STRING_LENGTH];
  char type[16];
  err = mapper->nextFile(&it, name, type);
    
  while (err != Mapper::MapperError::NO_MORE_FILES) {       
    if (err == Mapper::MapperError::OK) {
      response->printf("{\"type\":\"%s\",\"name\":\"%s\"}", type, name);         
    }
    err = mapper->nextFile(&it, name, type);
    if (err == Mapper::MapperError::OK) {
      response->println(",");
    }    
  }
  response->print("]");
  request->send(response);
}