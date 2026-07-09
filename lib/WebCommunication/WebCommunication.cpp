/**
 * @file WebCommunication.cpp
 * @brief Implementations of JSON formatting, sensor measurements, and input validations.
 */

#include "WebCommunication.h"
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "Config.h"
#include "Utilities.h"
#include "Pins.h"

#define MODULE_NAME "WebCommunication"

WebCommunication::WebCommunication() 
    : _ldrPin(PIN_LDR)
    , _ledPin(PIN_LED) {}

/**
 * @brief Sets up hardware pin modes.
 */
void WebCommunication::begin() {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
    pinMode(_ldrPin, INPUT);
    Utilities::log(MODULE_NAME, "GPIO initialized for LED and LDR sensor.");
}

/**
 * @brief Handles /lireLuminosite request.
 * @details Reads raw analog ADC voltage (0-4095 range on ESP32) and responds in JSON format.
 */
void WebCommunication::handleGetLuminosity(AsyncWebServerRequest *request) {
    int val = analogRead(_ldrPin);
    
    // Format JSON payload response
    String jsonResponse = "{\"status\":\"success\",\"value\":" + String(val) + "}";
    
    request->send(200, "application/json", jsonResponse);
    Utilities::logf(MODULE_NAME, "API /lireLuminosite called. Value: %d", val);
}

/**
 * @brief Handles LED state request.
 * @details Performs physical write on the LED pin and updates API client with active status.
 */
void WebCommunication::handleLedControl(AsyncWebServerRequest *request, bool state) {
    digitalWrite(_ledPin, state ? HIGH : LOW);
    
    String stateStr = state ? "ON" : "OFF";
    String jsonResponse = "{\"status\":\"success\",\"led\":\"" + stateStr + "\"}";
    
    request->send(200, "application/json", jsonResponse);
    Utilities::logf(MODULE_NAME, "API /%s called. Led set to %s", state ? "on" : "off", stateStr.c_str());
}

/**
 * @brief Intercepts undefined routes.
 * @details Captures DNS hijacked clients in AP mode and redirects them to the local hostname index.
 *          Otherwise, returns a clean 404 JSON formatted response.
 */
void WebCommunication::handleNotFound(AsyncWebServerRequest *request) {
    String host = request->host();
    
    // Redirect unknown hosts to captive portal domain name if in AP mode
    if (SELECTED_WIFI_MODE == WIFI_MODE_AP_SEL && !host.equalsIgnoreCase(LOCAL_DOMAIN) && !host.equals(WiFi.softAPIP().toString())) {
        String redirectUrl = "http://" + String(LOCAL_DOMAIN) + "/";
        request->redirect(redirectUrl);
        Utilities::logf(MODULE_NAME, "Redirected Host '%s' to '%s'", host.c_str(), redirectUrl.c_str());
        return;
    }
    
    request->send(404, "application/json", "{\"error\":\"Not Found\"}");
    Utilities::logf(MODULE_NAME, "Request not found: %s", request->url().c_str());
}

/**
 * @brief Validates input parameters in incoming queries to verify they correspond to real pins.
 */
bool WebCommunication::parseAndValidatePin(AsyncWebServerRequest *request, const String& paramName, int& outPin) {
    if (!request->hasParam(paramName)) {
        return false;
    }
    String paramValue = request->getParam(paramName)->value();
    int val = paramValue.toInt();
    
    // Validate GPIO range based on platform
#ifdef ESP32
    if (val < 0 || val > 39) {
        return false;
    }
#elif defined(ESP8266)
    if (val < 0 || val > 16) {
        return false;
    }
#endif
    outPin = val;
    return true;
}
