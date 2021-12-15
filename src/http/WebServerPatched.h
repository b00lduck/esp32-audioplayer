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
#pragma once
#include <WebServer.h>
#include "SdFat.h"
#include "config.h"

class WebServerPatched : public WebServer {
  public:   
    using WebServer::WebServer;
    inline size_t streamFile32(File32 &file, const String& contentType) {
        char name[MAX_FILENAME_STRING_BUFFER_LENGTH];
        file.getName(name, MAX_FILENAME_STRING_BUFFER_LENGTH);
        _streamFileCore(file.size(), name, contentType);
        return _currentClient.write(file);
    }  
};