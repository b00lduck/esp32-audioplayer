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
#include "Arduino.h"
#include <WebServer.h>
#include "rfid/rfid.h"
#include "rfid/ndef.h"
#include "config.h"
#include "storage/sd.h"
#include "storage/mapper.h"

#define MAX_UPLOAD_PATH_LEN 300

class HTTPServer {

  public: 
    HTTPServer(RFID *rfid, Mapper *mapper, SDCard *sd);    
    bool start(NDEF::WifiConfig *wificonfig);
    void shutdown();

  private:
    WebServer server;
    TaskHandle_t handleClientTaskHandle;

    RFID   *rfid;
    Mapper *mapper;
    SDCard *sd;

    void handlerNotFound();
    
    void handlerCurrentCardGet();  
    
    void handlerCardGet();    
    void handlerCardPost();                             
    void handlerCardPut();   
    
    void handlerFileGet();
    void handlerFileDelete();
    void handlerFileUpload();

    void sendError(int status, const char* msg);

    File uploadFile;

};
