#include "DomainManager.h"
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "Utilities.h"

#define MODULE_NAME "DomainManager"

DomainManager::DomainManager() 
    : _mode(WIFI_MODE_STA_SEL)
    , _dnsActive(false)
    , _lastIP("")
    , _wasConnected(false) {}

void DomainManager::begin(WifiModeSelection mode, const String& ipAddress) {
    _mode = mode;
    _lastIP = ipAddress;
    _wasConnected = (WiFi.status() == WL_CONNECTED || _mode == WIFI_MODE_AP_SEL);
    setupServices();
}

void DomainManager::setupServices() {
    String domain = LOCAL_DOMAIN;
    domain.replace("http://", "");
    domain.replace("https://", "");
    
    String mdnsHost = domain;
    if (mdnsHost.endsWith(".local")) {
        mdnsHost = mdnsHost.substring(0, mdnsHost.length() - 6);
    }
    
    if (_dnsActive) {
        _dnsServer.stop();
        _dnsActive = false;
    }
    
#ifdef ESP32
    MDNS.end();
#endif

    if (MDNS.begin(mdnsHost.c_str())) {
        Utilities::logf(MODULE_NAME, "mDNS responder started. Hostname: %s.local (IP: %s)", mdnsHost.c_str(), _lastIP.c_str());
        MDNS.addService("http", "tcp", HTTP_PORT);
    } else {
        Utilities::log(MODULE_NAME, "Error setting up MDNS responder!");
    }
    
    IPAddress currentIP;
    if (currentIP.fromString(_lastIP)) {
        _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        if (_mode == WIFI_MODE_AP_SEL) {
            bool success = _dnsServer.start(53, "*", currentIP);
            if (success) {
                Utilities::log(MODULE_NAME, "DNS Server started for captive portal redirect (AP mode).");
                _dnsActive = true;
            } else {
                Utilities::log(MODULE_NAME, "Failed to start DNS Server (AP).");
            }
        } else {
            bool success = _dnsServer.start(53, domain, currentIP);
            if (success) {
                Utilities::logf(MODULE_NAME, "DNS Server started to resolve '%s' locally (Station mode).", domain.c_str());
                _dnsActive = true;
            } else {
                Utilities::log(MODULE_NAME, "Failed to start DNS Server (Station).");
            }
        }
    }
}

void DomainManager::handle() {
    bool currentlyConnected = (WiFi.status() == WL_CONNECTED || _mode == WIFI_MODE_AP_SEL);
    String currentIP = (_mode == WIFI_MODE_AP_SEL) ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
    
    if (currentlyConnected && (!_wasConnected || currentIP != _lastIP)) {
        Utilities::log(MODULE_NAME, "IP changed or connection established. Restarting services...");
        _lastIP = currentIP;
        _wasConnected = true;
        setupServices();
    } else if (!currentlyConnected && _wasConnected) {
        Utilities::log(MODULE_NAME, "Wi-Fi connection lost. Stopping DNS...");
        if (_dnsActive) {
            _dnsServer.stop();
            _dnsActive = false;
        }
        _wasConnected = false;
    }

    if (_dnsActive) {
        _dnsServer.processNextRequest();
    }
#ifdef ESP8266
    MDNS.update();
#endif
}

