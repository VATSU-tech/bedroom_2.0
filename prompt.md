# Refonte complète du projet en architecture modulaire

Je souhaite que tu réalises une **refonte complète de mon projet** afin d'obtenir une architecture professionnelle, évolutive, réutilisable et facilement maintenable. Il ne s'agit pas simplement de déplacer du code dans plusieurs fichiers, mais de concevoir une véritable architecture modulaire respectant les bonnes pratiques du développement embarqué avec PlatformIO (ESP8266/ESP32 + Arduino Framework).

## Objectifs principaux

* Refactoriser entièrement le projet.
* Séparer clairement les responsabilités de chaque module (principe de responsabilité unique).
* Rendre chaque module indépendant et réutilisable dans d'autres projets.
* Améliorer considérablement la lisibilité, la maintenance et l'évolutivité du code.
* Éviter tout code dupliqué.
* Commenter les parties importantes.
* Utiliser des noms explicites et cohérents.

---

# Structure souhaitée

Je souhaite une organisation similaire à celle-ci :

```
Projet/
│
├── include/
│   ├── Config.h
│   ├── Definitions.h
│   ├── Pins.h
│   ├── Constants.h
│   └── Version.h
│
├── lib/
│   │
│   ├── NetworkManager/
│   │     ├── NetworkManager.h
│   │     └── NetworkManager.cpp
│   │
│   ├── WebServer/
│   │     ├── WebServer.h
│   │     └── WebServer.cpp
│   │
│   ├── DomainManager/
│   │     ├── DomainManager.h
│   │     └── DomainManager.cpp
│   │
│   ├── WebCommunication/
│   │     ├── WebCommunication.h
│   │     └── WebCommunication.cpp
│   │
│   ├── RFID/
│   │     ├── RFID.h
│   │     └── RFID.cpp
│   │
│   ├── Storage/
│   │     ├── Storage.h
│   │     └── Storage.cpp
│   │
│   ├── Utilities/
│   │     ├── Utilities.h
│   │     └── Utilities.cpp
│   │
│   └── ...
│
├── src/
│     └── main.cpp
│
└── data/
```

Cette structure n'est qu'un exemple. Tu peux l'améliorer si cela rend le projet plus propre.

---

# Fichier de configuration central

Je souhaite disposer d'un fichier unique dans **include/** (par exemple `Config.h` ou `Definitions.h`) contenant **tous les paramètres modifiables** du projet.

Je veux pouvoir modifier uniquement ce fichier sans devoir rechercher des constantes dans le reste du projet.

On devra notamment y trouver :

* Nom du projet
* Version
* Nom du point d'accès
* Mot de passe
* Adresse du domaine local
* Port HTTP
* Port HTTPS (si utilisé)
* Adresse IP fixe (si nécessaire)
* Paramètres DHCP
* Choix du mode Wi-Fi
* Délais
* Constantes diverses
* Activation ou désactivation de fonctionnalités
* Paramètres de débogage
* Paramètres RFID
* Paramètres réseau
* Tous les paramètres susceptibles d'être personnalisés

Aucune valeur importante ne doit être codée directement dans les modules.

---

# Module NetworkManager

Créer un véritable gestionnaire réseau.

Il devra pouvoir :

* gérer plusieurs réseaux Wi-Fi ;
* essayer automatiquement les réseaux dans l'ordre défini ;
* détecter lequel est disponible ;
* se reconnecter automatiquement en cas de perte ;
* afficher les informations de connexion ;
* gérer les délais de reconnexion ;
* fournir une API simple aux autres modules.

Par exemple :

```
Maison
Bureau
Téléphone
Hotspot
```

Le réseau disponible devra être sélectionné automatiquement.

---

# Choix du mode Wi-Fi

Dans le fichier de configuration, je souhaite simplement modifier une constante du type :

```
WIFI_MODE_STATION

ou

WIFI_MODE_AP
```

Le reste du projet devra s'adapter automatiquement.

Aucune modification ne devra être nécessaire ailleurs.

---

# Adresse unique

Que le projet fonctionne :

* en mode Station
* ou en mode Point d'accès

je souhaite utiliser **la même adresse** pour accéder à l'application.

Je ne veux plus taper une adresse IP du type :

```
192.168.x.x
```

Je souhaite utiliser une adresse plus conviviale, par exemple :

```
http://bedroom.local

ou

http://rfid.local

ou toute autre adresse définie dans le fichier de configuration.
```

Créer un module dédié (`DomainManager`) qui gère cette fonctionnalité.

Le nom de domaine devra être entièrement configurable.

---

# Communication Web ↔ Arduino

Créer un module entièrement dédié à la communication entre :

* l'interface Web
* et la carte Arduino/ESP.

Ce module devra gérer :

* la réception des données envoyées par le navigateur ;
* l'analyse (parsing) des requêtes ;
* la validation des données ;
* la conversion des formats si nécessaire ;
* l'envoi de réponses JSON ;
* l'organisation des routes API ;
* les échanges futurs.

Je souhaite que toute la logique de communication soit centralisée dans ce module.

---

# Serveur Web

Le serveur Web devra être séparé de la logique métier.

Il devra uniquement :

* initialiser le serveur ;
* enregistrer les routes ;
* servir les fichiers ;
* appeler les bons modules.

Aucune logique métier ne doit rester dans le serveur.

---

# Modularité

Chaque module doit :

* avoir un rôle unique ;
* exposer une interface propre ;
* masquer son implémentation ;
* pouvoir être copié dans un autre projet sans modification majeure.

Les dépendances entre modules doivent être réduites au minimum.

---

# Réutilisabilité

Je souhaite que les modules puissent être réutilisés dans mes futurs projets.

Par exemple :

* reprendre simplement le dossier `NetworkManager`
* ou `DomainManager`
* ou `RFID`

sans devoir modifier le reste du code.

---

# Lisibilité

Le code doit être :

* propre ;
* moderne ;
* bien indenté ;
* documenté ;
* facile à comprendre plusieurs mois plus tard.

Les noms des variables, fonctions, classes et fichiers doivent être explicites.

---

# Évolutivité

L'architecture doit permettre d'ajouter facilement de nouvelles fonctionnalités sans modifier les modules existants.

Par exemple :

* MQTT
* OTA
* WebSocket
* HTTPS
* Base de données
* Authentification
* Capteurs supplémentaires
* Écran OLED
* etc.

---

# Résultat attendu

Je ne veux pas une simple correction.

<!-- Je souhaite une **véritable refonte architecturale** respectant les bonnes pratiques de développement logiciel et les principes de modularité (SOLID lorsque cela est pertinent), afin d'obteni..r un projet professionnel, maintenable, évolutif et facilement réutilisable dans d'autres applications. -->
