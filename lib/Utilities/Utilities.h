#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

class Utilities {
public:
    static void initSerial();
    static String getMacAddress();
    static void log(const String& module, const String& message);
    static void logf(const String& module, const char* format, ...);
};

#endif // UTILITIES_H
