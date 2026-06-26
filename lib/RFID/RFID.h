#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

class RFID {
public:
    RFID();
    void begin();
    void handle();
    bool isCardPresent();
    String getLastCardUID();

private:
    unsigned long _lastReadTime;
    String _lastUID;
    bool _cardPresent;
};

#endif // RFID_H
