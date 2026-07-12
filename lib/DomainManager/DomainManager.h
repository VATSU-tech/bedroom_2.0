#ifndef DOMAIN_MANAGER_H
#define DOMAIN_MANAGER_H

#include <Arduino.h>
#ifdef ESP32
#include <ESPmDNS.h>
#elif defined(ESP8266)
#include <ESP8266mDNS.h>
#endif
#include <DNSServer.h>
#include "Config.h"

class DomainManager {
public:
    DomainManager();
    void begin(WifiModeSelection mode, const String& ipAddress);
    void handle();

private:
    void setupServices();

    WifiModeSelection _mode;
    DNSServer _dnsServer;
    bool _dnsActive;
    String _lastIP;
    bool _wasConnected;
};

#endif // DOMAIN_MANAGER_H
