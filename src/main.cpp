#include <Arduino.h>
#include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>

  #define ssid "Airtel_3031"  
  #define password "123456789000"  
  #define ssid_2 "DESKTOP-8NRTH60 va"
  #define password_2 "12345678900"

  ESP8266WebServer server(80);

  #define pin_led_1 D4

  bool etat_pin_led_1 = 0;

  void handleRoot();
  void handle_pin_led_1_on_off();

  void setup() 
  { Serial.begin(9600);
    WiFi.begin(ssid, password);

    pinMode(pin_led_1, OUTPUT);
    

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

    Serial.println("Connecté au Wi-Fi");
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());

    Serial.println("Initialisation du DFPlayer...");
    
    server.on("/", handleRoot);                                // Page principale
    server.on("/pin_led_1/on_off", handle_pin_led_1_on_off);   // Route pour allumer/eteintre LED 1
    
    server.begin();
    Serial.println("Serveur HTTP démarré");
  }

  void loop() {
    server.handleClient();  // Gestion des requêtes client
  }

  void handleRoot() {
    // Génération de la page web
    String htmlPage = "<html lang='fr'><title> VATSU tech </title>";
    htmlPage += "<style> body{ text-align:center; background-color:#000000;Color: #1FDBB7;font-family: cursive;padding:25vh 0;}";
    htmlPage += ".button_style {padding: 10px 20px; border-radius: 20px;border: none; color: white; text-align: center;";
    htmlPage += " text-decoration: none; display: inline-block;font-size: 25px; margin: 5px; cursor: pointer;";
    htmlPage += " transition: background-color 0.3s ease;background-color: #475C59; border-bottom-right-radius: 20px;";
    htmlPage += " box-shadow: 5px 5px 7px #000000; border-width: 2px; border-style: solid; border-color: #302C2C;}</style>";

    htmlPage += "<h1>VATSU TECH CONTROLEUR </h1>";

    htmlPage += "<p><a href=\"/pin_led_1/on_off\"><button class ='button_style' style ='background-color: ";
    htmlPage += (etat_pin_led_1)?"#00AAAA" : "#475C59;";
    htmlPage += "; color : white;'>LAMPE 1</button></a></p> ";

    htmlPage += "<p><h3> allumer ou etteindre des lampes</h3>";
    htmlPage += "<h2> sur un site web </h2></p>";
    htmlPage += "</body></html>";
    
    server.send(200, "text/html", htmlPage);  // Envoi de la page au client
  }

  void handle_pin_led_1_on_off() 
  { etat_pin_led_1 = !etat_pin_led_1;
    digitalWrite(pin_led_1, etat_pin_led_1);
    handleRoot();
  }