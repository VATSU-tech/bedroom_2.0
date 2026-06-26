/**
 * @file Utilities.h
 * @brief Header for log and debug utilities.
 * @details Contains centralized tools for message logging and serial initializations.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

/**
 * @class Utilities
 * @brief Static helper class for logging and board utility functions.
 */
class Utilities {
public:
    /**
     * @brief Initializes Serial port communication for print statements.
     */
    static void initSerial();

    /**
     * @brief Reads the physical MAC address of the Wi-Fi interface.
     * @return String containing MAC address (formatted as XX:XX:XX:XX:XX:XX).
     */
    static String getMacAddress();

    /**
     * @brief Writes a simple log line to the serial interface.
     * @param module Name of the calling system module (e.g. "Storage").
     * @param message Text string to display.
     */
    static void log(const String& module, const String& message);

    /**
     * @brief Writes a formatted log line (printf style) to the serial interface.
     * @param module Name of the calling system module.
     * @param format C-style format string.
     * @param ... Additional argument list corresponding to the format markers.
     */
    static void logf(const String& module, const char* format, ...);
};

#endif // UTILITIES_H
