#ifndef PINS_H
#define PINS_H

#ifdef ESP32
// GPIO Pin mappings for ESP32
const int PIN_LED = 2;
const int PIN_LDR = 34; // Capteur de luminosité

// Pin mappings for RFID (e.g. RC522) if enabled in the future
const int PIN_RFID_SS = 5;
const int PIN_RFID_RST = 22;
#elif defined(ESP8266)
// GPIO Pin mappings for ESP8266 (NodeMCU)
const int PIN_LED = 2;   // Built-in LED on NodeMCU is D4 (GPIO2), active low
const int PIN_LDR = A0;  // NodeMCU only has A0 as analog input

// Pin mappings for RFID (e.g. RC522) if enabled in the future
const int PIN_RFID_SS = 15;  // D8 (GPIO15)
const int PIN_RFID_RST = 16; // D0 (GPIO16)
#endif

#endif // PINS_H
