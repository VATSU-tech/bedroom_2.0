#include "WebServer.h"
#include "Storage.h"
#include "Config.h"
#include "Utilities.h"

#define MODULE_NAME "WebServer"

WebServer::WebServer(WebCommunication& webComm) 
    : _server(HTTP_PORT)
    , _webComm(webComm) {}

void WebServer::begin() {
    registerRoutes();
    _server.begin();
    Utilities::logf(MODULE_NAME, "Web Server started on port %d", HTTP_PORT);
}

void WebServer::registerRoutes() {
    // Serve static files from Storage (SPIFFS)
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(Storage::getFS(), "/index.html", "text/html");
    });

    _server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(Storage::getFS(), "/w3.css", "text/css");
    });

    _server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(Storage::getFS(), "/script.js", "text/javascript");
    });

    // API Routes mapped to WebCommunication module
    _server.on("/lireLuminosite", HTTP_GET, [this](AsyncWebServerRequest *request) {
        _webComm.handleGetLuminosity(request);
    });

    _server.on("/on", HTTP_GET, [this](AsyncWebServerRequest *request) {
        _webComm.handleLedControl(request, true);
    });

    _server.on("/off", HTTP_GET, [this](AsyncWebServerRequest *request) {
        _webComm.handleLedControl(request, false);
    });

    // Handle captive portal/unknown requests
    _server.onNotFound([this](AsyncWebServerRequest *request) {
        _webComm.handleNotFound(request);
    });
}
