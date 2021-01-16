/**
 * 
 * Copyright 2018 D.Zerlett <daniel@zerlett.eu>
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
#include "Arduino.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "rfid.h"
#include "config.h"
#include "../storage/sd.h"
#include "../storage/mapper.h"

class HTTP {

  public: 
    HTTP(RFID *rfid, Mapper *mapper, SDCard *sd);
    void init();   

  private:
    AsyncWebServer server;

    RFID   *rfid;
    Mapper *mapper;
    SDCard *sd;

    #ifdef ENABLE_CORS
      void addCorsHeader(AsyncWebServerResponse *response);
      void handlerCors(AsyncWebServerRequest *request);
    #endif 

    void handlerCurrentCardGet(AsyncWebServerRequest *request);  
    void handlerCardGet(AsyncWebServerRequest *request);
    void handlerFileGet(AsyncWebServerRequest *request);
    
    bool uploadInProgress;
    char uploadPath[264]; // "/upload/<max 255>" 1+6+1+255+1=264
    File uploadFile;

};


