/**
 * @file NetworkManager.cpp
 * @brief Implementations of Wi-Fi setups, fallbacks, and non-blocking reconnect handling.
 */

#include "NetworkManager.h"
#include "Utilities.h"

#define MODULE_NAME "NetworkManager"

NetworkManager::NetworkManager()
    : _mode(SELECTED_WIFI_MODE)
    , _lastReconnectAttempt(0)
    , _currentNetworkIndex(0)
    , _isConnected(false) {}

/**
 * @brief Starts Wi-Fi interface.
 * @details Forces clean state by clearing old persistent config.
 */
void NetworkManager::begin() {
    WiFi.persistent(false);
    WiFi.disconnect(true);
    
    if (_mode == WIFI_MODE_STA_SEL) {
        initStationMode();
    } else {
        initAPMode();
    }
}

/**
 * @brief Sets up ESP32 as a Wi-Fi client (Station mode).
 * @details Configures static IP settings if USE_STATIC_IP is true in Config.h.
 */
void NetworkManager::initStationMode() {
    Utilities::log(MODULE_NAME, "Starting in STATION mode...");
    WiFi.mode(WIFI_MODE_STA);
    
    if (USE_STATIC_IP) {
        if (WiFi.config(STATIC_IP, STATIC_GATEWAY, STATIC_SUBNET, STATIC_DNS)) {
            Utilities::log(MODULE_NAME, "Static IP configured successfully.");
        } else {
            Utilities::log(MODULE_NAME, "Static IP configuration failed!");
        }
    }
    
    _currentNetworkIndex = 0;
    _isConnected = false;
    
    if (WIFI_NETWORKS_COUNT == 0) {
        Utilities::log(MODULE_NAME, "No Wi-Fi networks configured in Config.h!");
        return;
    }
    
    // Attempt connecting to the primary Wi-Fi config
    tryConnectToNetwork(WIFI_NETWORKS[_currentNetworkIndex]);
}

/**
 * @brief Sets up ESP32 as a soft Access Point (hotspot mode).
 */
void NetworkManager::initAPMode() {
    Utilities::log(MODULE_NAME, "Starting in ACCESS POINT mode...");
    WiFi.mode(WIFI_MODE_AP);
    
    bool success = WiFi.softAP(AP_SSID, AP_PASSWORD, AP_CHANNEL, AP_HIDDEN, AP_MAX_CONNECTION);
    if (success) {
        Utilities::log(MODULE_NAME, "Access Point active!");
        Utilities::logf(MODULE_NAME, "SSID: %s", AP_SSID);
        Utilities::logf(MODULE_NAME, "IP Address: %s", WiFi.softAPIP().toString().c_str());
        _isConnected = true;
    } else {
        Utilities::log(MODULE_NAME, "Failed to start Access Point.");
        _isConnected = false;
    }
}

/**
 * @brief Connection handler that waits synchronously up to connection timeout.
 */
bool NetworkManager::tryConnectToNetwork(const WifiNetwork& net) {
    Utilities::logf(MODULE_NAME, "Attempting to connect to SSID: %s", net.ssid);
    WiFi.begin(net.ssid, net.password);
    
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_CONNECTION_TIMEOUT_MS) {
        delay(250);
        #if ENABLE_DEBUG
        Serial.print(".");
        #endif
    }
    #if ENABLE_DEBUG
    Serial.println();
    #endif
    
    if (WiFi.status() == WL_CONNECTED) {
        Utilities::logf(MODULE_NAME, "Connected successfully to SSID: %s", net.ssid);
        Utilities::logf(MODULE_NAME, "IP Address: %s", WiFi.localIP().toString().c_str());
        _isConnected = true;
        return true;
    } else {
        Utilities::logf(MODULE_NAME, "Failed to connect to SSID: %s", net.ssid);
        _isConnected = false;
        return false;
    }
}

/**
 * @brief Non-blocking handle loop called to manage connections.
 * @details Retries alternate Wi-Fi credentials in a circular loop if connection drops.
 */
void NetworkManager::handle() {
    if (_mode == WIFI_MODE_AP_SEL) {
        // Nothing to maintain in AP mode
        return;
    }
    
    // Monitor connection states
    if (WiFi.status() == WL_CONNECTED) {
        if (!_isConnected) {
            _isConnected = true;
            Utilities::logf(MODULE_NAME, "Reconnected to SSID: %s", WiFi.SSID().c_str());
            Utilities::logf(MODULE_NAME, "IP Address: %s", WiFi.localIP().toString().c_str());
        }
    } else {
        if (_isConnected) {
            Utilities::log(MODULE_NAME, "Wi-Fi connection lost!");
            _isConnected = false;
            _lastReconnectAttempt = millis();
        }
        
        // Périodically trigger reconnect attempt using round-robin backup networks
        if (millis() - _lastReconnectAttempt >= WIFI_RECONNECT_INTERVAL_MS) {
            _lastReconnectAttempt = millis();
            
            _currentNetworkIndex = (_currentNetworkIndex + 1) % WIFI_NETWORKS_COUNT;
            Utilities::logf(MODULE_NAME, "Retrying with network index %d...", _currentNetworkIndex);
            tryConnectToNetwork(WIFI_NETWORKS[_currentNetworkIndex]);
        }
    }
}

bool NetworkManager::isConnected() const {
    return _isConnected;
}

String NetworkManager::getIPAddress() const {
    if (_mode == WIFI_MODE_AP_SEL) {
        return WiFi.softAPIP().toString();
    } else {
        return WiFi.localIP().toString();
    }
}

WifiModeSelection NetworkManager::getActiveMode() const {
    return _mode;
}
