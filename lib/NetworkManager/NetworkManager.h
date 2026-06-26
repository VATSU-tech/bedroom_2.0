#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "Config.h"

class NetworkManager {
public:
    NetworkManager();
    void begin();
    void handle();
    
    bool isConnected() const;
    String getIPAddress() const;
    WifiModeSelection getActiveMode() const;

private:
    void initStationMode();
    void initAPMode();
    bool tryConnectToNetwork(const WifiNetwork& net);
    
    WifiModeSelection _mode;
    unsigned long _lastReconnectAttempt;
    int _currentNetworkIndex;
    bool _isConnected;
};

#endif // NETWORK_MANAGER_H
