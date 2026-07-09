/**
 * @file NetworkManager.h
 * @brief Header for Wi-Fi connection manager.
 * @details Manages multi-network connections, reconnects, static IP setups in STA mode,
 *          and SoftAP configurations in AP mode.
 */

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "Config.h"

/**
 * @class NetworkManager
 * @brief Manages the board Wi-Fi connection state and configuration.
 */
class NetworkManager {
public:
    /**
     * @brief Constructor initializing default variables.
     */
    NetworkManager();

    /**
     * @brief Initiates Wi-Fi connection in Station or Access Point mode.
     */
    void begin();

    /**
     * @brief Monitored loop handler. Call this inside loop() to handle auto-reconnections.
     */
    void handle();
    
    /**
     * @brief Returns the Wi-Fi connection status.
     * @return true if connected to AP (STA mode) or SoftAP is active (AP mode).
     */
    bool isConnected() const;

    /**
     * @brief Gets the active IP Address as a string.
     * @return String containing IP Address (Local IP or softAP IP).
     */
    String getIPAddress() const;

    /**
     * @brief Retrieves the active Wi-Fi mode.
     * @return Selected enum value (WIFI_MODE_STA_SEL or WIFI_MODE_AP_SEL).
     */
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
