/**
 * @file RFID.cpp
 * @brief Implementations of RFID checks conditioned on configuration flags.
 */

#include "RFID.h"
#include "Config.h"
#include "Utilities.h"

#define MODULE_NAME "RFID"

RFID::RFID() 
    : _lastReadTime(0)
    , _lastUID("")
    , _cardPresent(false) {}

/**
 * @brief Performs interface setup if ENABLE_RFID is true.
 */
void RFID::begin() {
#if ENABLE_RFID
    Utilities::log(MODULE_NAME, "Initializing RFID module...");
    // Put SPI and RC522 setup logic here
#else
    Utilities::log(MODULE_NAME, "RFID module disabled in configuration.");
#endif
}

/**
 * @brief Scans for new tags in a non-blocking frequency interval.
 */
void RFID::handle() {
#if ENABLE_RFID
    if (millis() - _lastReadTime >= RFID_READ_INTERVAL_MS) {
        _lastReadTime = millis();
        // Check SPI/RC522 reader status, set flags, read UID
    }
#endif
}

bool RFID::isCardPresent() {
    return _cardPresent;
}

String RFID::getLastCardUID() {
    return _lastUID;
}
