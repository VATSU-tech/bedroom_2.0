#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "WebCommunication.h"

class WebServer {
public:
    WebServer(WebCommunication& webComm);
    void begin();

private:
    AsyncWebServer _server;
    WebCommunication& _webComm;
    
    void registerRoutes();
};

#endif // WEB_SERVER_H
