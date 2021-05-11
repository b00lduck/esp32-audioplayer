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

void HTTPServer::handlerFileUpload() {

  String path = server.urlDecode(server.pathArg(0));
 
  HTTPUpload& upload = server.upload();
   String absoluteFilename = path + "/" + upload.filename;

  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("[HTTP] Starting file upload to folder %s\n", path.c_str());

    bool created = SD.mkdir(path);
    if (created) {
      Serial.printf("[HTTP] File upload: directory %s was created\n", path.c_str());
    } else {
      Serial.printf("[HTTP] File upload: directory %s was not created\n", path.c_str());
    }

    if (SD.exists(absoluteFilename)) {
      Serial.printf("[HTTP] File already exists, deleting %s\n", absoluteFilename.c_str());
      SD.remove(absoluteFilename);
    }

    uploadFile = SD.open(absoluteFilename, "w");
    if (!uploadFile) {
      sendError(500, "File upload error: could not open file");
      return;
    }
    uploadFile.close();

  } else if (upload.status == UPLOAD_FILE_WRITE) {    

    uploadFile = SD.open(absoluteFilename, "a");   
    if (uploadFile) {
      size_t written = uploadFile.write(upload.buf, upload.currentSize);
      
      if (written != upload.currentSize) {
        Serial.printf("File upload error: could not write file (bytes expected: %d, bytes written: %d, total: %d)\n", upload.currentSize, written, upload.totalSize);
      }
      uploadFile.close();
    } else {
      Serial.printf("Could not open file for writing! %d\n", upload.totalSize);
    } 

  } else if (upload.status == UPLOAD_FILE_END) {

    Serial.println("File upload finished");

  }

}

void HTTPServer::handlerFileGet() {  
  
  String path = server.urlDecode(server.pathArg(0));
 
  if (!path.equals("/") && path.endsWith("/")) {
    path = path.substring(0,path.length()-1);
  }

  File file = SD.open(path);
  if (!SD.exists(path) || (!file)) {
    sendError(404, "File could not be found on SD-card");
    return;
  }  

  if (file.isDirectory()) {

    String ret = "[";
    
    char name[MAX_FILENAME_STRING_BUFFER_LENGTH];
    char type[16];
    size_t size;
    
    Mapper::MapperError err = mapper->nextFile(&file, name, type, &size);
      
    while (err != Mapper::MapperError::NO_MORE_FILES) {       
      if (err == Mapper::MapperError::OK) {        
        ret += "{\"type\":\"";
        ret += type;
        ret += "\",\"name\":\"";
        ret += name;
        ret += "\",\"size\":\"";
        ret += size;
        ret += "\"}";
      }
      err = mapper->nextFile(&file, name, type, &size);
      if (err == Mapper::MapperError::OK) {
        ret += ",";
      }    
    }

    ret += "]";
    server.send(200, "application/json", ret);

  } else {                   
    Serial.printf("[HTTP] Streaming %s\n", path.c_str());
    server.streamFile(file, "application/octet-stream");    
  }

  file.close();
}

void HTTPServer::handlerFileDelete() {  
  String path = server.urlDecode(server.pathArg(0));
  Serial.printf("[HTTP] Deleting %s\n", path.c_str());
  
  if (path.length() == 0) {    
    sendError(400, "Path length can not be 0");
    return;
  }

  if (!SD.exists(path)) {    
    sendError(404, "File could not be found on SD-card");
    return;
  }
  
  if (!SD.remove(path)) {
    sendError(500, "Error deleting file on SD-card");
    return;
  }

  server.send(204);
}
