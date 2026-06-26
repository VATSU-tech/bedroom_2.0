#ifndef DOMAIN_MANAGER_H
#define DOMAIN_MANAGER_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include "Config.h"

class DomainManager {
public:
    DomainManager();
    void begin(WifiModeSelection mode, const String& ipAddress);
    void handle();

private:
    WifiModeSelection _mode;
    DNSServer _dnsServer;
    bool _dnsActive;
};

#endif // DOMAIN_MANAGER_H
