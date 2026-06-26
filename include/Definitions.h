#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Wi-Fi operational modes
enum WifiModeSelection {
    WIFI_MODE_STA_SEL,
    WIFI_MODE_AP_SEL
};

// Wi-Fi Station Credentials Structure
struct WifiNetwork {
    const char* ssid;
    const char* password;
};

#endif // DEFINITIONS_H
