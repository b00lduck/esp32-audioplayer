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
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("[HTTP] Starting file upload to folder %s\n", path);
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = path + "/" + filename;
    }
    uploadFile = SD.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {    
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
    }
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
    
    Mapper::MapperError err = mapper->nextFile(&file, name, type);
      
    while (err != Mapper::MapperError::NO_MORE_FILES) {       
      if (err == Mapper::MapperError::OK) {        
        ret += "{\"type\":\"";
        ret += type;
        ret += "\",\"name\":\"";
        ret += name;
        ret += "\"}";
      }
      err = mapper->nextFile(&file, name, type);
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
