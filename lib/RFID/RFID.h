/**
 * @file RFID.h
 * @brief Header for skeletal RFID reader module.
 * @details Included for architectural scalability, demonstrating simple clean integration 
 *          of additional peripherals inside the new modular design.
 */

#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

/**
 * @class RFID
 * @brief Handles peripheral RFID scanning and tag check.
 */
class RFID {
public:
    /**
     * @brief Constructor initializing default values.
     */
    RFID();

    /**
     * @brief Starts the physical interface (SPI, RC522 library setup, etc.) if enabled.
     */
    void begin();

    /**
     * @brief Monitored loop handler. Must be called in loop() to read cards periodically.
     */
    void handle();

    /**
     * @brief Informs if a tag was recently detected.
     * @return true if card was read, false otherwise.
     */
    bool isCardPresent();

    /**
     * @brief Reads the UID tag of the last scanned card.
     * @return String containing card identifier.
     */
    String getLastCardUID();

private:
    unsigned long _lastReadTime;
    String _lastUID;
    bool _cardPresent;
};

#endif // RFID_H
