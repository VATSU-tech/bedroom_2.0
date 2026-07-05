# 🚀 bedroom ESP32 - Architecture Modulaire & Interface Premium

Ce projet est une refonte complète et professionnelle d'une application IoT pour ESP32 développée sous **PlatformIO**. L'architecture monolithique d'origine a été restructurée en modules de responsabilité unique, découplés, réutilisables et hautement documentés.

Le projet propose un tableau de bord Web moderne en *dark-mode* avec effets de verre (*glassmorphism*), requêtes asynchrones via l'API Fetch, gestion robuste du Wi-Fi (multi-réseaux et point d'accès), et redirection DNS (portail captif).

---

## 📋 Table des Matières

1. [Architecture & Structure du Projet](#-architecture--structure-du-projet)
2. [Description Détaillée des Fichiers](#-description-détaillée-des-fichiers)
3. [Processus et Mise en œuvre avec PlatformIO](#-processus-et-mise-en-œuvre-avec-platformio)
4. [Configuration du Système](#-configuration-du-système)
5. [Fonctionnement de l'Interface Web & API](#-fonctionnement-de-linterface-web--api)
6. [Dépannage & Commandes Utiles](#-dépannage--commandes-utiles)

---

## 🏗️ Architecture & Structure du Projet

L'arborescence respecte les standards PlatformIO en séparant les entêtes globaux, les modules réutilisables (librairies internes), et le code applicatif principal :

```
vatsu_bedroom_2.0/
│
├── include/                   # Entêtes et configurations globales
│   ├── Config.h               # Paramètres modifiables (Wi-Fi, domaines, options)
│   ├── Definitions.h          # Types et ennumérations personnalisés
│   ├── Pins.h                 # Assignation des GPIOs de la carte
│   ├── Constants.h            # Délais et constantes système
│   └── Version.h              # Version du micrologiciel
│
├── lib/                       # Modules applicatifs (compilés séparément)
│   ├── Utilities/             # Logs centralisés (Serial)
│   ├── Storage/               # Gestionnaire de fichiers flash (SPIFFS)
│   ├── NetworkManager/        # Connexion Wi-Fi intelligente (STA/AP) et reconnexions
│   ├── DomainManager/         # Résolution d'adresse (mDNS) & DNS local (Captive Portal)
│   ├── WebCommunication/      # Logiciels d'API, validation et réponses JSON
│   ├── WebServer/             # Serveur HTTP asynchrone (routage et fichiers statiques)
│   └── RFID/                  # Squelette de module d'identification RFID
│
├── src/
│   └── main.cpp               # Point d'entrée principal (setup et loop simplifiés)
│
├── data/                      # Ressources du site web (servies via SPIFFS)
│   ├── index.html             # Interface utilisateur sémantique
│   ├── w3.css                 # Style esthétique et responsive (Dark Premium CSS)
│   └── script.js              # Script AJAX moderne (API Fetch)
│
└── platformio.ini             # Fichier de configuration PlatformIO du projet
```

---

## 📄 Description Détaillée des Fichiers

### 1. Dossier `include/` (Configuration Globale)

* **[Config.h](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/include/Config.h)** : C'est le **cœur de contrôle** du projet. Il permet d'activer ou de désactiver des fonctionnalités (par exemple `ENABLE_RFID`), de configurer le mode Wi-Fi actif, de déclarer une liste ordonnée de réseaux Wi-Fi de secours, et de spécifier le nom de domaine convivial (`http://bedroom.local`).
* **[Definitions.h](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/include/Definitions.h)** : Définit les types génériques comme la structure de stockage des identifiants Wi-Fi et les états configurables.
* **[Pins.h](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/include/Pins.h)** : Mappe de manière lisible les ports de l'ESP32. La LED est sur le pin 2 et le capteur analogique de luminosité (LDR) sur la broche 34.
* **[Constants.h](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/include/Constants.h)** : Regroupe les valeurs numériques figées comme le débit en bauds série (115200) ou les délais de scrutation matériels.
* **[Version.h](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/include/Version.h)** : Stocke la chaîne de versioning logicielle.

### 2. Dossier `lib/` (Librairies Métiers)

* **[Utilities](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/lib/Utilities/Utilities.h)** : Fournit un mécanisme d'affichage console formaté, identifiant automatiquement le module appelant (ex : `[Storage] SPIFFS mounted successfully.`).
* **[Storage](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/lib/Storage/Storage.h)** : Encapsule la gestion du système de fichiers `SPIFFS`. Il s'assure du bon montage de la mémoire flash à l'allumage et peut lister le contenu complet de la mémoire.
* **[NetworkManager](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/lib/NetworkManager/NetworkManager.h)** : Gère de façon autonome la liaison réseau :
  * *Mode Station (STA)* : Il essaie de s'associer aux SSID configurés dans l'ordre défini. Si le réseau tombe, il tente de se reconnecter périodiquement de manière non-bloquante.
  * *Mode Point d'Accès (AP)* : Il monte son propre hotspot Wi-Fi sécurisé avec l'adresse IP par défaut `192.168.4.1`.
* **[DomainManager](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/lib/DomainManager/DomainManager.h)** : Assure la convivialité d'accès :
  * Démarre un répondeur **mDNS** pour résoudre l'adresse `.local` (ex : `http://bedroom.local`) sur les terminaux compatibles.
  * Démarre un **serveur DNS captif** en mode AP pour capturer toutes les requêtes DNS externes et renvoyer l'adresse IP de l'ESP32.
* **[WebCommunication](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/lib/WebCommunication/WebCommunication.h)** : Gère les appels API HTTP. Il isole la manipulation des GPIOs et des capteurs et retourne les résultats au format standardisé JSON (`{"status":"success", "value":1024}`).
* **[WebServer](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/lib/WebServer/WebServer.h)** : Contient le serveur Web asynchrone. Il associe les requêtes d'URL statiques (HTML, CSS, JS) à la mémoire flash `SPIFFS` et délègue les chemins d'API `/on`, `/off`, `/lireLuminosite` au module `WebCommunication`.
* **[RFID](file:///media/work/Scripts/IoT/PlatformIO/vatsu_bedroom_2.0/lib/RFID/RFID.h)** : Module squelette conçu pour l'intégration future de capteurs d'identification (ex : RC522) afin d'assurer l'évolutivité.

---

## 🛠️ Processus et Mise en œuvre avec PlatformIO

Voici le déroulé complet étape par étape pour configurer, compiler et téléverser ce projet modulaire sur votre carte ESP32.

### Étape 1 : Fichier de configuration `platformio.ini`

PlatformIO compile chaque sous-dossier de `lib/` de manière isolée. Pour permettre à ces bibliothèques d'accéder aux fichiers d'entêtes globaux situés dans `include/`, nous configurons l'environnement avec le drapeau `-I include` dans `platformio.ini` :

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

# Dépendances externes nécessaires
lib_deps =
  esp32async/ESPAsyncWebServer @ ^3.11.1
  esp32async/AsyncTCP @ ^3.4.10

# Drapeau critique pour la structure modulaire
build_flags = -I include
```

### Étape 2 : Compilation du Micrologiciel (Firmware)

Le compilateur assemble le fichier `src/main.cpp` ainsi que toutes les bibliothèques locales du dossier `lib/` :

* **Via l'interface VS Code** : Cliquez sur l'icône de PlatformIO (tête de fourmi), puis cliquez sur **Build**.
* **Via le terminal de commande (CLI)** :

  ```bash
  pio run
  ```

### Étape 3 : Téléversement du Micrologiciel (Upload)

Transfère le binaire compilé (`firmware.bin`) dans la mémoire programme de l'ESP32 :

* **Interface VS Code** : Cliquez sur **Upload**.
* **Via le terminal** :

  ```bash
  pio run --target upload
  ```

### Étape 4 : Téléversement des fichiers Web (Upload SPIFFS/Filesystem)

C'est une étape cruciale pour ce projet. Le site Web (HTML, CSS, JS) situé dans le dossier `data/` doit être écrit dans la mémoire flash de l'ESP32 afin d'être servi par le module `Storage`.

* **Interface VS Code** : Allez dans l'onglet PlatformIO, déroulez `Platform` et cliquez sur **Upload Filesystem Image**.
* **Via le terminal** :

  ```bash
  pio run --target uploadfs
  ```

### Étape 5 : Monitorer la console Série

Pour lire les logs produits par le module `Utilities` :

* **Interface VS Code** : Cliquez sur l'icône de prise électrique ou **Monitor**.
* **Via le terminal** :

  ```bash
  pio device monitor
  ```

---

## ⚙️ Configuration du Système

Pour adapter le projet à vos besoins, ouvrez uniquement **`include/Config.h`** :

```cpp
// 1. Choix du mode Wi-Fi (WIFI_MODE_STA_SEL ou WIFI_MODE_AP_SEL)
#define SELECTED_WIFI_MODE WIFI_MODE_STA_SEL

// 2. Définition des réseaux Wi-Fi (STA) triés par préférence
const WifiNetwork WIFI_NETWORKS[] = {
    {"Nom_De_Votre_Wifi", "Cle_Securite"},
    {"Wifi_De_Secours", "Cle_Secours"}
};

// 3. Configuration du Hotspot (Mode AP)
#define AP_SSID "ESP32_bedroom"
#define AP_PASSWORD "12345678"

// 4. Adresse de domaine conviviale
#define LOCAL_DOMAIN "bedroom.local"

// 5. Activation des modules complémentaires
#define ENABLE_DEBUG true
#define ENABLE_RFID false
```

---

## 🌐 Fonctionnement de l'Interface Web & API

### 📶 Captive Portal & DNS local

* **En mode Station** : Connectez votre ordinateur/smartphone sur le même réseau Wi-Fi. Tapez `http://bedroom.local` dans votre navigateur.
* **En mode Point d'Accès** : Connectez-vous au réseau Wi-Fi diffusé par la carte (`ESP32_bedroom`). Le serveur DNS capte toutes les requêtes. Ouvrez n'importe quel site internet (ex : `http://test.com`) ou tapez `http://bedroom.local` pour être redirigé vers l'interface.

### 🛡️ API JSON standardisée

Le dialogue entre le code JavaScript du navigateur (`script.js`) et la carte s'effectue via des requêtes AJAX asynchrones :

1. `GET /lireLuminosite` ➡️ Renvoie la valeur analogique lue par le capteur de lumière :

   ```json
   { "status": "success", "value": 412 }
   ```

2. `GET /on` ➡️ Allume la LED matérielle et répond :

   ```json
   { "status": "success", "led": "ON" }
   ```

3. `GET /off` ➡️ Éteint la LED et répond :

   ```json
   { "status": "success", "led": "OFF" }
   ```

---

## 🔍 Dépannage & Commandes Utiles

### Le site affiche "Erreur SPIFFS..." ou ne charge pas les styles

👉 Vous avez probablement oublié d'uploader les fichiers statiques. Exécutez la commande : `pio run -t uploadfs`.

### L'adresse `.local` ne s'ouvre pas

* Assurez-vous d'avoir saisi `http://` devant le nom de domaine (`http://bedroom.local`).
* Certains systèmes d'exploitation (notamment Android) ne prennent pas en charge mDNS par défaut. Si vous êtes en mode Station, essayez depuis un PC Windows/macOS/Linux, ou utilisez l'IP attribuée affichée dans le moniteur série.
* En mode AP, tout nom de domaine (comme `http://bedroom.local`) sera redirigé automatiquement grâce au portail captif DNS.
