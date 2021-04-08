#include "AsyncSDFileResponse.h"

void AsyncSDFileResponse::_setContentType(const String& path){
  if (path.endsWith(".html")) _contentType = "text/html";
  else if (path.endsWith(".htm")) _contentType = "text/html";
  else if (path.endsWith(".css")) _contentType = "text/css";
  else if (path.endsWith(".json")) _contentType = "text/json";
  else if (path.endsWith(".js")) _contentType = "application/javascript";
  else if (path.endsWith(".png")) _contentType = "image/png";
  else if (path.endsWith(".gif")) _contentType = "image/gif";
  else if (path.endsWith(".jpg")) _contentType = "image/jpeg";
  else if (path.endsWith(".ico")) _contentType = "image/x-icon";
  else if (path.endsWith(".svg")) _contentType = "image/svg+xml";
  else if (path.endsWith(".eot")) _contentType = "font/eot";
  else if (path.endsWith(".woff")) _contentType = "font/woff";
  else if (path.endsWith(".woff2")) _contentType = "font/woff2";
  else if (path.endsWith(".ttf")) _contentType = "font/ttf";
  else if (path.endsWith(".xml")) _contentType = "text/xml";
  else if (path.endsWith(".pdf")) _contentType = "application/pdf";
  else if (path.endsWith(".zip")) _contentType = "application/zip";
  else if(path.endsWith(".gz")) _contentType = "application/x-gzip";
  else _contentType = "text/plain";
}

AsyncSDFileResponse::AsyncSDFileResponse(SDFS &sd, const String& path, const String& contentType){
  _code = 200;
  _path = path;
  
  _content = sd.open(_path, FILE_READ);
  _contentLength = _content.size();
  _sourceIsValid = _content;

  if(contentType == "") {
    setContentType(path);
  } else {
    _contentType = contentType;
  }   
}

size_t AsyncSDFileResponse::_fillBuffer(uint8_t *data, size_t len) {
  Serial.printf("fillBuffer %d\n", len);  
  delay(10);
  return _content.read(data, len);
}

AsyncSDFileResponse::~AsyncSDFileResponse(){
  if(_content) {
    _content.close();
  }
}

