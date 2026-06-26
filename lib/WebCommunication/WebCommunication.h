#ifndef WEB_COMMUNICATION_H
#define WEB_COMMUNICATION_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class WebCommunication {
public:
    WebCommunication();
    void begin();
    
    // Handlers for WebServer routes
    void handleGetLuminosity(AsyncWebServerRequest *request);
    void handleLedControl(AsyncWebServerRequest *request, bool state);
    void handleNotFound(AsyncWebServerRequest *request);
    
    // Future expansion helper (e.g., parsing JSON requests or query parameters)
    bool parseAndValidatePin(AsyncWebServerRequest *request, const String& paramName, int& outPin);

private:
    int _ldrPin;
    int _ledPin;
};

#endif // WEB_COMMUNICATION_H
