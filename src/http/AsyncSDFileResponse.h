#pragma once
#include <ESPAsyncWebServer.h>
#include "SD.h"

class AsyncSDFileResponse: public AsyncAbstractResponse {
  private:
    File _content;
    String _path;
    void _setContentType(const String& path);
    bool _sourceIsValid;
  public:
    AsyncSDFileResponse(SDFS &sd, const String& path, const String& contentType=String());
    ~AsyncSDFileResponse();
    bool _sourceValid() const { return _sourceIsValid; } 
    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};
