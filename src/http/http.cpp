/**
 * 
 * Copyright 2018-2020 D.Zerlett <daniel@zerlett.eu>
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
#include "config_wifi.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

HTTP::HTTP(RFID *rfid, Mapper *mapper, SDCard *sd) : 
  server(80),
  rfid(rfid),
  mapper(mapper),
  sd(sd)
{} 

void notFound(AsyncWebServerRequest *request) {
    request->send(404, F("text/plain"), F("Not found"));
}

#ifdef ENABLE_CORS
void HTTP::addCorsHeader(AsyncWebServerResponse *response) {
  response->addHeader(F("Access-Control-Allow-Origin"),"*");
  response->addHeader(F("Access-Control-Allow-Methods"),"*");
  response->addHeader(F("Access-Control-Allow-Headers"),"*");
}

void HTTP::handlerCors(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response = request->beginResponse(204);
  #ifdef ENABLE_CORS
    addCorsHeader(response);
  #endif
  request->send(response);
}
#endif

void HTTP::init() {

  // connect to WiFi
  WiFi.disconnect(true, true);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(F("Connecting to WiFi.."));
  }  
  Serial.println(WiFi.localIP());

  // create server and routes
  const char cardUrl[] = "/api/card";
  server.on(cardUrl, HTTP_GET, std::bind(&HTTP::handlerCardGet, this, std::placeholders::_1));

  const char currentCardUrl[] = "/api/card/current"; 
  server.on(currentCardUrl, HTTP_GET, std::bind(&HTTP::handlerCurrentCardGet, this, std::placeholders::_1));

  const char fileUrl[] = "^\\/api\\/file(\\/.*)*$";
  server.on(fileUrl, HTTP_GET, std::bind(&HTTP::handlerFileGet, this, std::placeholders::_1));

  #ifdef ENABLE_CORS
    server.on(".*", HTTP_OPTIONS, std::bind(&HTTP::handlerCors, this, std::placeholders::_1));
  #endif

  server.onFileUpload([this](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index) {
      Serial.printf("UploadStart: %s\n", filename.c_str());
      uploadInProgress = true;
      snprintf(uploadPath, 264, "/upload/%s", filename.c_str());
      uploadFile = SD.open(uploadPath, FILE_WRITE);      
    }
    
    Serial.printf("Writing %d bytes at %d to %s\n", len, index, uploadPath);
    uploadFile.seek(index);
    uploadFile.write(data, len);
    
    if(final) {   
      Serial.printf("UploadEnd: %s (%u)\n", uploadPath, index+len);
      uploadFile.close();
      uploadInProgress = false;
    }
  });

  server.onNotFound(notFound);
  server.begin(); 
}
