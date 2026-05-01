#include <Arduino.h>
#include <ESP8266WiFi.h>
  #include <SoftwareSerial.h>
  #include <ESP8266WebServer.h>
  #include <DFRobotDFPlayerMini.h>

  #define ssid "Airtel-3031"  
  #define password "123456789"  
  #define ssid_2 "DESKTOP-8NRTH60 va"
  #define password_2 "12345678900"

  SoftwareSerial mySerial(D6, D7); // RX, TX

  DFRobotDFPlayerMini myDFPlayer;
  ESP8266WebServer server(80);

  const int pin_led_1 = D0;
  const int pin_led_2 = D1;

  bool etat_pin_led_1 = 0;
  bool etat_pin_led_2 = 0;
  bool activation_allumage_generale   = 0;
  bool activation_extinction_generale = 0;


  void setup() 
  { Serial.begin(115200);
    mySerial.begin(9600);  // Vitesse standard du DFPlayer
    WiFi.begin(ssid_2, password_2);

    pinMode(pin_led_1, OUTPUT);
    pinMode(pin_led_2, OUTPUT);

    digitalWrite(pin_led_1, etat_pin_led_1);
    digitalWrite(pin_led_2, etat_pin_led_2);
    
    Serial.println("Connecté au Wi-Fi");
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

    Serial.println("Initialisation du DFPlayer...");

    // if (!myDFPlayer.begin(mySerial)) {
    //   Serial.println("Erreur DFPlayer : Vérifie les connexions.");
    //   while (true);
    // }

    Serial.println("DFPlayer prêt.");

    myDFPlayer.volume(30);  // Volume (0 à 30)
    
    server.on("/", handleRoot);                                // Page principale
    server.on("/pin_led_1/on_off", handle_pin_led_1_on_off);   // Route pour allumer/eteintre LED 1
    server.on("/pin_led_2/on_off", handle_pin_led_2_on_off);   // Route pour allumer/eteintre LED 1
    server.on("/all_pin_leds/all_off", handle_all_pin_leds_off);   // Route pour allumer/eteintre LED 3
    server.on("/all_pin_leds/all_on" , handle_all_pin_leds_on );   // Route pour allumer/eteintre LED 3
    
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

    htmlPage += "<p><a href=\"/pin_led_2/on_off\"><button class ='button_style' style ='background-color: ";
    htmlPage += (etat_pin_led_2)?"#00AAAA" : "#475C59;";
    htmlPage += "; color : white;'>LAMPE 2</button></a></p>";

    htmlPage += "<p><a href=\"/all_pin_leds/all_off\"><button class ='button_style' style ='background-color: ";
    htmlPage += (activation_extinction_generale)?"#000000" : "#475C59;";
    htmlPage += "; color : white;'>ALL OFF</button></a>";

    htmlPage += "<a href=\"/all_pin_leds/all_on\"><button class ='button_style'style ='background-color: ";
    htmlPage += (activation_allumage_generale)?"#EEFFEE" : "#475C59;";
    htmlPage += "; color : black;'>ALL   ON</button></a></p>";

    htmlPage += "<p><h3> allumer ou etteindre des lampes</h3>";
    htmlPage += "<h2> sur un site web </h2></p>";
    htmlPage += "</body></html>";
    
    server.send(200, "text/html", htmlPage);  // Envoi de la page au client
  }

  void handle_pin_led_1_on_off() 
  { etat_pin_led_1 = !etat_pin_led_1;
    activation_allumage_generale = 0;
    activation_extinction_generale=0;
    digitalWrite(pin_led_1, etat_pin_led_1);
    (etat_pin_led_1) ? myDFPlayer.play(1) : myDFPlayer.play(3);
    handleRoot();
  }
  void handle_pin_led_2_on_off() 
  { etat_pin_led_2 = !etat_pin_led_2;
    activation_allumage_generale = 0;
    myDFPlayer.next();
    // (etat_pin_led_2) ? myDFPlayer.play(2) : myDFPlayer.play(4);
    activation_extinction_generale=0;
    digitalWrite(pin_led_2, etat_pin_led_2 ? HIGH : LOW);
    handleRoot();
  }
  void handle_all_pin_leds_off()
  { etat_pin_led_1 = 0;
    etat_pin_led_2 = 0;
    activation_allumage_generale   = 0;
    activation_extinction_generale = 1;

    // myDFPlayer.play(6);
    myDFPlayer.volumeDown();
    digitalWrite(pin_led_1 , etat_pin_led_1);
    digitalWrite(pin_led_2 , etat_pin_led_1);
    handleRoot();
  }
  void handle_all_pin_leds_on()
  { etat_pin_led_1 = 1;
    etat_pin_led_2 = 1;
    activation_allumage_generale   = 1;
    activation_extinction_generale = 0;

    // myDFPlayer.play(5);
    myDFPlayer.volumeUp();
    digitalWrite(pin_led_1 , etat_pin_led_1);
    digitalWrite(pin_led_2 , etat_pin_led_1);
    handleRoot();
  }