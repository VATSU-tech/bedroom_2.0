#include "DomainManager.h"
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "Utilities.h"

#define MODULE_NAME "DomainManager"

DomainManager::DomainManager() : _mode(WIFI_MODE_STA_SEL), _dnsActive(false) {}

void DomainManager::begin(WifiModeSelection mode, const String& ipAddress) {
    _mode = mode;
    
    // Clean up domain name (remove http:// and .local for setting up mDNS)
    String domain = LOCAL_DOMAIN;
    domain.replace("http://", "");
    domain.replace("https://", "");
    
    String mdnsHost = domain;
    if (mdnsHost.endsWith(".local")) {
        mdnsHost = mdnsHost.substring(0, mdnsHost.length() - 6);
    }
    
    // Initialize mDNS
    if (MDNS.begin(mdnsHost.c_str())) {
        Utilities::logf(MODULE_NAME, "mDNS responder started. Hostname: %s.local", mdnsHost.c_str());
        MDNS.addService("http", "tcp", HTTP_PORT);
    } else {
        Utilities::log(MODULE_NAME, "Error setting up MDNS responder!");
    }
    
    // If in AP mode, start DNS server to redirect all requests to ESP IP
    if (_mode == WIFI_MODE_AP_SEL) {
        IPAddress apIP;
        apIP.fromString(ipAddress);
        
        // Port 53 is the standard DNS port
        _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        bool success = _dnsServer.start(53, "*", apIP);
        if (success) {
            Utilities::log(MODULE_NAME, "DNS Server started for captive portal redirect.");
            _dnsActive = true;
        } else {
            Utilities::log(MODULE_NAME, "Failed to start DNS Server.");
            _dnsActive = false;
        }
    }
}

void DomainManager::handle() {
    if (_dnsActive) {
        _dnsServer.processNextRequest();
    }
#ifdef ESP8266
    MDNS.update();
#endif
}
