/**
 * @file WebServer.cpp
 * @brief Implementations of routing configurations and file mounts.
 */

#include "WebServer.h"
#include "Storage.h"
#include "Config.h"
#include "Utilities.h"

#define MODULE_NAME "WebServer"

WebServer::WebServer(WebCommunication& webComm) 
    : _server(HTTP_PORT)
    , _webComm(webComm) {}

/**
 * @brief Registers handlers and starts the server daemon.
 */
void WebServer::begin() {
    registerRoutes();
    _server.begin();
    Utilities::logf(MODULE_NAME, "Web Server started on port %d", HTTP_PORT);
}

/**
 * @brief Maps static paths and binds API callbacks.
 * @details Static files (HTML, CSS, JS) are served directly from the LittleFS partition 
 *          accessible via the Storage module reference.
 */
void WebServer::registerRoutes() {
    // Serve HTML page
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(Storage::getFS(), "/index.html", "text/html");
    });

    // Serve CSS file
    _server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(Storage::getFS(), "/w3.css", "text/css");
    });

    // Serve Javascript client script
    _server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(Storage::getFS(), "/script.js", "text/javascript");
    });

    // Bind API Routes to WebCommunication methods
    _server.on("/lireLuminosite", HTTP_GET, [this](AsyncWebServerRequest *request) {
        _webComm.handleGetLuminosity(request);
    });

    _server.on("/on", HTTP_GET, [this](AsyncWebServerRequest *request) {
        _webComm.handleLedControl(request, true);
    });

    _server.on("/off", HTTP_GET, [this](AsyncWebServerRequest *request) {
        _webComm.handleLedControl(request, false);
    });

    // Fallback handler for not found / redirection requests
    _server.onNotFound([this](AsyncWebServerRequest *request) {
        _webComm.handleNotFound(request);
    });
}
