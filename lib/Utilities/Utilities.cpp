/**
 * @file Utilities.cpp
 * @brief Implementations of serial initialization and formatted logging functions.
 */

#include "Utilities.h"
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "Config.h"

/**
 * @brief Start hardware Serial and print standard banner.
 * @details Retries up to 3 seconds for Native USB Serial lines (like ESP32-S3/C3) to activate.
 */
void Utilities::initSerial() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 3000) {
        delay(10);
    }
    Serial.println("\n=== SYSTEM STARTUP ===");
}

/**
 * @brief Fetch the Wi-Fi MAC address.
 */
String Utilities::getMacAddress() {
    return WiFi.macAddress();
}

/**
 * @brief Outputs standard log formatted as: [ModuleName] Message
 * @details The logs are only compiled and sent to serial if ENABLE_DEBUG is true in Config.h.
 */
void Utilities::log(const String& module, const String& message) {
#if ENABLE_DEBUG
    Serial.print("[");
    Serial.print(module);
    Serial.print("] ");
    Serial.println(message);
#endif
}

/**
 * @brief Outputs formatted string log using vsnprintf buffer.
 * @details Safeguards overflow using a fixed 256-character output buffer.
 */
void Utilities::logf(const String& module, const char* format, ...) {
#if ENABLE_DEBUG
    Serial.print("[");
    Serial.print(module);
    Serial.print("] ");
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    Serial.println(buffer);
#endif
}
