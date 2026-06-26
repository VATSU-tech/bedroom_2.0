#include <Arduino.h>
#include "Config.h"
#include "Utilities.h"
#include "Storage.h"
#include "NetworkManager.h"
#include "DomainManager.h"
#include "WebCommunication.h"
#include "WebServer.h"
#include "RFID.h"

// Instantiate modules
NetworkManager networkManager;
DomainManager domainManager;
WebCommunication webComm;
WebServer webServer(webComm);
RFID rfid;

void setup() {
    // Initialize system utilities (Serial, etc.)
    Utilities::initSerial();
    Utilities::log("Main", "Initializing modules...");

    // Initialize Storage (filesystem)
    if (!Storage::begin()) {
        Utilities::log("Main", "Critical error: Storage initialization failed!");
    }

    // Initialize Network (Station/AP mode according to Config)
    networkManager.begin();

    // Initialize Domain Name (mDNS / Captive Portal DNS)
    domainManager.begin(networkManager.getActiveMode(), networkManager.getIPAddress());

    // Initialize Web Communication APIs
    webComm.begin();

    // Start Web Server
    webServer.begin();

    // Initialize RFID module
    rfid.begin();

    Utilities::log("Main", "Initialization complete! System ready.");
}

void loop() {
    // Maintain module run loops
    networkManager.handle();
    domainManager.handle();
    rfid.handle();
    
    // Tiny delay to yield time to underlying ESP RTOS & prevent watchdog reset
    delay(true);
}