/**
 * @file main.cpp
 * @brief Application Entry Point for the bedroom ESP32 project.
 * @details This file implements the main startup initializations and runtime loops.
 *          By utilizing a modular design, this file only serves to orchestrate 
 *          and trigger handlers inside the individual system modules.
 * @author VATSU-tech
 * @version 2.0.0
 * @date 2026-06-26
 */

#include <Arduino.h>
#include "Config.h"
#include "Utilities.h"
#include "Storage.h"
#include "NetworkManager.h"
#include "DomainManager.h"
#include "WebCommunication.h"
#include "WebServer.h"
#include "RFID.h"

// ==========================================
// Module Instantiations
// ==========================================
/// Network Manager handles Wi-Fi connections in Station or Access Point modes.
NetworkManager networkManager;

/// Domain Manager sets up mDNS responder and Captive Portal DNS redirection.
DomainManager domainManager;

/// WebCommunication processes API payloads and drives physical pins.
WebCommunication webComm;

/// WebServer routes HTTP requests (web pages and REST API routes).
WebServer webServer(webComm);

/// RFID abstracts card reader peripheral routines.
RFID rfid;

// ==========================================
// Main Initializations
// ==========================================
/**
 * @brief Setup routine called once on microcontroller boot.
 * @details Initializes serial communication, mounts the SPIFFS filesystem partition,
 *          initiates the Wi-Fi client or hotspot, sets up domain routing,
 *          configures web servers, and starts auxiliary sensors.
 */
void setup() {
    // 1. Initialize serial port for debug logs
    Utilities::initSerial();
    Utilities::log("Main", "Initializing modules...");

    // 2. Mount and list files from SPIFFS flash partition
    if (!Storage::begin()) {
        Utilities::log("Main", "Critical error: Storage initialization failed!");
    }

    // 3. Connect to Wi-Fi STA or launch SoftAP
    networkManager.begin();

    // 4. Initialize hostname resolutions (mDNS & Captive DNS fallback)
    domainManager.begin(networkManager.getActiveMode(), networkManager.getIPAddress());

    // 5. Initialize API GPIO states
    webComm.begin();

    // 6. Bind handlers and start HTTP Server
    webServer.begin();

    // 7. Start card reader module
    rfid.begin();

    Utilities::log("Main", "Initialization complete! System ready.");
}

// ==========================================
// Runtime Loop
// ==========================================
/**
 * @brief Main execution loop called repeatedly.
 * @details Delegates runtime tasks to individual manager loops.
 *          Calls are non-blocking to prevent watchdog resets.
 */
void loop() {
    // 1. Maintain Wi-Fi connection states and fallback attempts
    networkManager.handle();

    // 2. Handle incoming captive portal DNS requests
    domainManager.handle();

    // 3. Scan for card identifiers
    rfid.handle();
    
    // Tiny delay to yield time to underlying ESP RTOS & prevent watchdog reset
    delay(1);
}