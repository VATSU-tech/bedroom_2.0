/**
 * @file WebCommunication.h
 * @brief Header for Web API communication logic.
 * @details Translates HTTP incoming payloads into native GPIO controls, reads LDR values,
 *          validates request parameters, and formats output strings into JSON payload format.
 */

#ifndef WEB_COMMUNICATION_H
#define WEB_COMMUNICATION_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

/**
 * @class WebCommunication
 * @brief Isolates logic handlers mapping to API routes from the WebServer setup.
 */
class WebCommunication {
public:
    /**
     * @brief Constructor initializing default values.
     */
    WebCommunication();

    /**
     * @brief Initializes GPIO pins for sensors and actuators controlled by the API.
     */
    void begin();
    
    /**
     * @brief API Handler for `/lireLuminosite`. Reads LDR and sends JSON response.
     * @param request Reference to client HTTP request.
     */
    void handleGetLuminosity(AsyncWebServerRequest *request);

    /**
     * @brief API Handler for LED controls (`/on` & `/off`).
     * @param request Reference to client HTTP request.
     * @param state Output pin target logic state (true = HIGH, false = LOW).
     */
    void handleLedControl(AsyncWebServerRequest *request, bool state);

    /**
     * @brief Handles captive portal redirection and 404 responses.
     * @param request Reference to client HTTP request.
     */
    void handleNotFound(AsyncWebServerRequest *request);
    
    /**
     * @brief Generic validation helper verifying query param parameters represent valid pins.
     * @param request Reference to client HTTP request.
     * @param paramName Key of the parameter to fetch.
     * @param outPin Reference to store parsed result value if valid.
     * @return true if parameter existed and matches valid ESP32 GPIO range, false otherwise.
     */
    bool parseAndValidatePin(AsyncWebServerRequest *request, const String& paramName, int& outPin);

private:
    int _ldrPin;
    int _ledPin;
};

#endif // WEB_COMMUNICATION_H
