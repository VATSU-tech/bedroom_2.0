#include "Utilities.h"
#include <WiFi.h>
#include "Config.h"

void Utilities::initSerial() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 3000) {
        delay(10);
    }
    Serial.println("\n--- system startup ---");
}

String Utilities::getMacAddress() {
    return WiFi.macAddress();
}

void Utilities::log(const String& module, const String& message) {
#if ENABLE_DEBUG
    Serial.print("[");
    Serial.print(module);
    Serial.print("] ");
    Serial.println(message);
#endif
}

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
