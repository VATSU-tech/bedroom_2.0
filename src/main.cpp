#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// LED Pin (GPIO 2 is the built-in LED on most ESP32 Dev Modules)
const int ledPin = 2;

// LDR / Analog sensor pin (GPIO 34 is an input-only ADC pin on ESP32)
const int ldrPin = 34;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);

  // Initialize LED Pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Initialize SPIFFS
  // Setting the formatOnFail parameter to true will format SPIFFS if mount fails
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted successfully");

  // --- OPTION 1: Set up Access Point (AP Mode) ---
  // Connect to SSID: "ESP32-DevModule" with password: "password123"
  // Server will be accessible at http://192.168.4.1
  WiFi.softAP("ESP32-DevModule", "password123");
  Serial.print("Access Point started. IP Address: ");
  Serial.println(WiFi.softAPIP());

  // --- OPTION 2: Station Mode (STA Mode) ---
  // To connect to your home Wi-Fi instead, uncomment this block and comment out the AP Mode lines.
  /*
  const char* ssid = "YOUR_WIFI_SSID";
  const char* password = "YOUR_WIFI_PASSWORD";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  */

  // --- WEB SERVER ROUTES ---

  // Route to serve index.html at root "/"
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Route to turn the LED on
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED turned ON");
    request->send(200, "text/plain", "LED ON");
  });

  // Route to turn the LED off
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledPin, LOW);
    Serial.println("LED turned OFF");
    request->send(200, "text/plain", "LED OFF");
  });

  // Route to read brightness value
  server.on("/lireLuminosite", HTTP_GET, [](AsyncWebServerRequest *request) {
    int value = analogRead(ldrPin);
    request->send(200, "text/plain", String(value));
  });

  // Serve other static files in the SPIFFS (e.g. w3.css, script.js)
  // This automatically serves files from SPIFFS that match their URI path
  server.serveStatic("/", SPIFFS, "/");

  // Start the server
  server.begin();
  Serial.println("HTTP Server started");
}

void loop() {
  // ESPAsyncWebServer operates asynchronously, so nothing is needed here.
}