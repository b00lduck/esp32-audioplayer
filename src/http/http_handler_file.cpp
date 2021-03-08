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

void HTTP::handlerFilePost(AsyncWebServerRequest *request) {  
  AsyncWebServerResponse *response = request->beginResponse(204);      
  request->send(response);
}

void HTTP::handlerFilePostUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {  
  if(!index){
    if (uploadInProgress) {
      Serial.println("upload already in progress");
      request->send(409);
      return;
    }
    if (filename.length() > 255) {
      Serial.println("filename too long");
      request->send(400);
      return;
    }
    
    const char *path = request->pathArg(0).c_str();
    if ((!path) || (strlen(path) == 0)) {
      Serial.println("no path given");
      request->send(400);
      return;
    }

    Serial.printf("UploadStart: %s\n", filename.c_str());
    uploadInProgress = true;    
    snprintf(uploadPath, 300, "/%s/%s", path, filename.c_str());
    if (strlen(uploadPath) > MAX_UPLOAD_PATH_LEN) {
      Serial.println("upload path too long");
      request->send(400);
      return;
    }

    uploadFile = SD.open(uploadPath, FILE_WRITE);      
  }
  
  Serial.print(".");
  uploadFile.seek(index);
  uploadFile.write(data, len);

  if(final){    
    Serial.printf("\nUploadEnd: %s, %u B\n", filename.c_str(), index+len);
    uploadFile.close();
    uploadInProgress = false;
  }

}

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


void HTTP::handlerFileDelete(AsyncWebServerRequest *request) {  

  const char *path = request->pathArg(0).c_str();

  AsyncWebServerResponse *response; 

  if (strlen(path) == 0) {
    response = request->beginResponse(400);
  } else if (!SD.exists(path)) {
    Serial.printf("File not found: %s\n", path);
    response = request->beginResponse(404);
  } else if (!SD.remove(path)) {
    Serial.printf("Error deleting file: %s\n", path);
    response = request->beginResponse(500);
  } else {
    Serial.printf("Deleted file: %s\n", path);
    response = request->beginResponse(200);
  }

  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif
  request->send(response);
}
