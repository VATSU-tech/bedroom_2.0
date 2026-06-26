#include "RFID.h"
#include "Config.h"
#include "Utilities.h"

#define MODULE_NAME "RFID"

RFID::RFID() 
    : _lastReadTime(0)
    , _lastUID("")
    , _cardPresent(false) {}

void RFID::begin() {
#if ENABLE_RFID
    Utilities::log(MODULE_NAME, "Initializing RFID module...");
    // Setup SPI pins and MFRC522 instance here if needed
#else
    Utilities::log(MODULE_NAME, "RFID module disabled in configuration.");
#endif
}

void RFID::handle() {
#if ENABLE_RFID
    if (millis() - _lastReadTime >= RFID_READ_INTERVAL_MS) {
        _lastReadTime = millis();
        // Check for card presence, read UID
    }
#endif
}

bool RFID::isCardPresent() {
    return _cardPresent;
}

String RFID::getLastCardUID() {
    return _lastUID;
}
