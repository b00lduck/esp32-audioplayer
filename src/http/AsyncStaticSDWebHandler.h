#pragma once
#include <SD.h>
#include <ESPAsyncWebServer.h>

class AsyncStaticSDWebHandler: public AsyncWebHandler {

  private:
    bool _getFile(AsyncWebServerRequest *request);
    bool _fileExists(AsyncWebServerRequest *request, const String& path);

  protected:
    SDFS _sd;
    String _uri;
    String _path;
    bool _isDir;

  public:
    AsyncStaticSDWebHandler(const char* uri, SDFS& sd, const char* path, const char* cache_control = NULL);
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;

};
