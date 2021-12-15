/**
 * 
 * Copyright 2018-2021 D.Zerlett <daniel@zerlett.eu>
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
#include <WiFi.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>
#include "WebServerPatched.h"

void handleClientTask(void *parameter){
  while (true) {
    WebServerPatched *server = (WebServerPatched*)parameter;
    server->handleClient();
    delay(2);
  } 
}

HTTPServer::HTTPServer(RFID *rfid, Mapper *mapper, SDCard *sdCard) : 
  server(80),
  rfid(rfid),
  mapper(mapper),
  sdCard(sdCard) {} 

void HTTPServer::handlerNotFound() {
    server.send(404, "text/plain", "not found");  
}

bool HTTPServer::start(NDEF::WifiConfig *wifiConfig) {    

  WiFi.persistent(false);
  WiFi.disconnect(true, true);

  uint8_t triesLeft = 30;

  // try connection to WiFi
  WiFi.begin(wifiConfig->SSID, wifiConfig->password);
  Serial.printf("Connection to %s with secret %s\n", wifiConfig->SSID, wifiConfig->password);
  while ((WiFi.status() != WL_CONNECTED) && (triesLeft)){
    delay(1000);
    Serial.print(F("."));
    triesLeft--;
  }  

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Could not connect to WiFi"));
    return false;
  }

  Serial.println(WiFi.localIP());  


  const UriRegex everything = UriRegex("^.*$");
  server.on(everything, HTTP_OPTIONS, [this]() {
    server.send(200, "", "");
  });

  // create server and routes
  const Uri currentCardUrl = Uri("/api/card/current");
  server.on(currentCardUrl, HTTP_GET, std::bind(&HTTPServer::handlerCurrentCardGet, this));

  const UriRegex cardWithIdUrl = UriRegex("^\\/api\\/card\\/([0-9a-fA-F]{8})$");
  server.on(cardWithIdUrl, HTTP_POST, std::bind(&HTTPServer::handlerCardPost, this));
  server.on(cardWithIdUrl, HTTP_PUT, std::bind(&HTTPServer::handlerCardPut, this));

  const Uri cardUrl = Uri("/api/card");
  server.on(cardUrl, HTTP_GET, std::bind(&HTTPServer::handlerCardGet, this));  

  const UriRegex fileUrl = UriRegex("^\\/api\\/file(\\/.*)$");
  server.on(fileUrl, HTTP_GET, std::bind(&HTTPServer::handlerFileGet, this));
  server.on(fileUrl, HTTP_DELETE, std::bind(&HTTPServer::handlerFileDelete, this));
  server.on(fileUrl, HTTP_POST, [this]() {
    server.send(201, "text/plain", "");
  }, std::bind(&HTTPServer::handlerFileUpload, this));

  server.onNotFound(std::bind(&HTTPServer::handlerNotFound, this));
  server.begin(); 
  
  #ifdef ENABLE_CORS
    server.enableCORS(true);
  #endif

  xTaskCreate(handleClientTask, "", 20000, &server, 1, &handleClientTaskHandle);

  return true;
}

void HTTPServer::sendError(int status, const char* msg) {
  Serial.printf("[HTTP] %d %s\n", status, msg);
  server.send(status, "text/plain", String(msg));
}

void HTTPServer::shutdown() {
  vTaskDelete(handleClientTaskHandle);
  server.close();
  WiFi.persistent(false);
  WiFi.disconnect(true, true);
  Serial.println(F("Stopped HTTP server and disconnected WiFi"));
}
