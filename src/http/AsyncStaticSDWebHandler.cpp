#include "AsyncStaticSDWebHandler.h"
#include "AsyncSDFileResponse.h"

AsyncStaticSDWebHandler::AsyncStaticSDWebHandler(const char* uri, SDFS& sd, const char* path, const char* cache_control)
  : _sd(sd), _uri(uri), _path(path) {
  // Ensure leading '/'
  if (_uri.length() == 0 || _uri[0] != '/') _uri = "/" + _uri;
  if (_path.length() == 0 || _path[0] != '/') _path = "/" + _path;

  // If path ends with '/' we assume a hint that this is a directory to improve performance.
  // However - if it does not end with '/' we, can't assume a file, path can still be a directory.
  _isDir = _path[_path.length()-1] == '/';

  // Remove the trailing '/' so we can handle default file
  // Notice that root will be "" not "/"
  if (_uri[_uri.length()-1] == '/') _uri = _uri.substring(0, _uri.length()-1);
  if (_path[_path.length()-1] == '/') _path = _path.substring(0, _path.length()-1);
}

bool AsyncStaticSDWebHandler::canHandle(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_GET && request->url().startsWith(_uri) && _getFile(request)) {
    return true;
  }
  return false;
}

bool AsyncStaticSDWebHandler::_getFile(AsyncWebServerRequest *request) {
  // Remove the found uri
  String path = request->url().substring(_uri.length());

  // We can skip the file check and look for default if request is to the root of a directory or that request path ends with '/'
  bool canSkipFileCheck = (_isDir && path.length() == 0) || (path.length() && path[path.length()-1] == '/');

  path = _path + path;

  // Do we have a file or .gz file
  if (!canSkipFileCheck && _fileExists(request, path))
    return true;

  return _fileExists(request, path);
}

bool AsyncStaticSDWebHandler::_fileExists(AsyncWebServerRequest *request, const String& path) {
  bool found = _sd.exists(path);
  if (found) {
    // Extract the file name from the path and keep it in _tempObject
    size_t pathLen = path.length();
    char * _tempPath = (char*)malloc(pathLen+1);
    snprintf(_tempPath, pathLen+1, "%s", path.c_str());
    request->_tempObject = (void*)_tempPath;
  }
  return found;
}

void AsyncStaticSDWebHandler::handleRequest(AsyncWebServerRequest *request) {
  
  // Get the filename from request->_tempObject and free it
  String filename = String((char*)request->_tempObject);
  free(request->_tempObject);
  request->_tempObject = NULL;

  if (_sd.exists(filename)) {
    AsyncWebServerResponse * response = new AsyncSDFileResponse(_sd, filename);
    request->send(response);
  } else {
    request->send(404);
  }
  
}