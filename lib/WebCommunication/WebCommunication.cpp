#include "WebCommunication.h"
#include <WiFi.h>
#include "Config.h"
#include "Utilities.h"
#include "Pins.h"

#define MODULE_NAME "WebCommunication"

WebCommunication::WebCommunication() 
    : _ldrPin(PIN_LDR)
    , _ledPin(PIN_LED) {}

void WebCommunication::begin() {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
    pinMode(_ldrPin, INPUT);
    Utilities::log(MODULE_NAME, "GPIO initialized for LED and LDR sensor.");
}

void WebCommunication::handleGetLuminosity(AsyncWebServerRequest *request) {
    int val = analogRead(_ldrPin);
    
    // Create a JSON response
    String jsonResponse = "{\"status\":\"success\",\"value\":" + String(val) + "}";
    
    request->send(200, "application/json", jsonResponse);
    Utilities::logf(MODULE_NAME, "API /lireLuminosite called. Value: %d", val);
}

void WebCommunication::handleLedControl(AsyncWebServerRequest *request, bool state) {
    digitalWrite(_ledPin, state ? HIGH : LOW);
    
    String stateStr = state ? "ON" : "OFF";
    String jsonResponse = "{\"status\":\"success\",\"led\":\"" + stateStr + "\"}";
    
    request->send(200, "application/json", jsonResponse);
    Utilities::logf(MODULE_NAME, "API /%s called. Led set to %s", state ? "on" : "off", stateStr.c_str());
}

void WebCommunication::handleNotFound(AsyncWebServerRequest *request) {
    // If client requests page in AP mode redirect to local domain or serve index.html (Captive Portal support)
    String host = request->host();
    
    // If not matching our domain, redirect to local domain (helpful for captive portal)
    if (SELECTED_WIFI_MODE == WIFI_MODE_AP_SEL && !host.equalsIgnoreCase(LOCAL_DOMAIN) && !host.equals(WiFi.softAPIP().toString())) {
        String redirectUrl = "http://" + String(LOCAL_DOMAIN) + "/";
        request->redirect(redirectUrl);
        Utilities::logf(MODULE_NAME, "Redirected Host '%s' to '%s'", host.c_str(), redirectUrl.c_str());
        return;
    }
    
    request->send(404, "application/json", "{\"error\":\"Not Found\"}");
    Utilities::logf(MODULE_NAME, "Request not found: %s", request->url().c_str());
}

bool WebCommunication::parseAndValidatePin(AsyncWebServerRequest *request, const String& paramName, int& outPin) {
    if (!request->hasParam(paramName)) {
        return false;
    }
    String paramValue = request->getParam(paramName)->value();
    int val = paramValue.toInt();
    if (val < 0 || val > 39) { // Simple validation for ESP32 pins
        return false;
    }
    outPin = val;
    return true;
}
