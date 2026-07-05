#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <IPAddress.h>
#include "Definitions.h"
#include "Pins.h"
#include "Constants.h"
#include "Version.h"

// ==========================================
// Project Metadata
// ==========================================
#define PROJECT_NAME "VatsuRoom"

// ==========================================
// Feature Toggles & Debugging
// ==========================================
#define ENABLE_DEBUG true
#define ENABLE_RFID false

// ==========================================
// Wi-Fi Configuration
// ==========================================
// Choice of mode: WIFI_MODE_STA_SEL or WIFI_MODE_AP_SEL
#define SELECTED_WIFI_MODE WIFI_MODE_STA_SEL

// --- Wi-Fi Station (STA) Mode ---
// Define the credentials of available Wi-Fi networks tried in order
const WifiNetwork WIFI_NETWORKS[] = {
    {"Airtel_3031", "123456789000"},
    {"DELTA-FORCE-5G", "L2info@reserve=+"},
};
const size_t WIFI_NETWORKS_COUNT = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);

// --- Wi-Fi Access Point (AP) Mode ---
#define AP_SSID "ESP32 VA room"
#define AP_PASSWORD "12345678"
#define AP_CHANNEL 1
#define AP_HIDDEN 0
#define AP_MAX_CONNECTION 4

// --- IP Custom Configuration (for STA mode) ---
#define USE_STATIC_IP false
const IPAddress STATIC_IP(192, 168, 1, 200);
const IPAddress STATIC_GATEWAY(192, 168, 1, 1);
const IPAddress STATIC_SUBNET(255, 255, 255, 0);
const IPAddress STATIC_DNS(8, 8, 8, 8);

// ==========================================
// Local Domain & Server Configuration
// ==========================================
// Custom domain name (friendly address) accessible in both AP and Station mode
#define LOCAL_DOMAIN "bedroom.local"
#define HTTP_PORT 80

#endif // CONFIG_H
