/**
 * @file WebServer.h
 * @brief Header for WebServer routing container.
 * @details Integrates the HTTP server instance, serves static files from Storage,
 *          and maps API calls to handlers inside the WebCommunication module.
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "WebCommunication.h"

/**
 * @class WebServer
 * @brief Initializes and configures paths for the HTTP server.
 */
class WebServer {
public:
    /**
     * @brief Constructor binding the WebCommunication API helper reference.
     * @param webComm Reference to the WebCommunication logic controller instance.
     */
    WebServer(WebCommunication& webComm);

    /**
     * @brief Configures routes and begins listening for HTTP requests.
     */
    void begin();

private:
    AsyncWebServer _server;
    WebCommunication& _webComm;
    
    void registerRoutes();
};

#endif // WEB_SERVER_H
