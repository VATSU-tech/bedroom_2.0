# 🧪 Guide Pédagogique du Test Unitaire sur PlatformIO

Bienvenue dans le dossier des tests ! Si vous n'avez jamais écrit de tests unitaires pour l'IoT ou le code embarqué, ce guide est fait pour vous. Nous allons tout expliquer de manière très simple, étape par étape.

---

## 🎯 Qu'est-ce qu'un Test Unitaire ?

Imaginez que vous construisez une maison. Avant de poser le toit, vous voulez vous assurer que chaque brique est solide. 

En programmation, c'est la même chose :
* Une **"unité"** est le plus petit morceau de code testable (une fonction, une classe).
* Un **"test unitaire"** est un petit programme automatique qui va appeler votre code avec des valeurs connues et vérifier s'il répond correctement.

> [!TIP]
> **Pourquoi faire cela ?**
> Si plus tard vous modifiez le code de votre projet pour ajouter une fonctionnalité, vous risquez de casser (sans le vouloir) une ancienne fonctionnalité. En lançant vos tests unitaires, vous découvrirez immédiatement où se situe le problème !

---

## 🛠️ Le Framework "Unity"

PlatformIO utilise une bibliothèque très légère appelée **Unity** pour exécuter les tests unitaire sur microcontrôleur (ESP32, ESP8266, etc.).

Chaque fichier de test suit la même structure qu'un programme Arduino classique, avec un `setup()` et un `loop()`.

### Les 4 étapes clés d'un fichier de test :
1. **`UNITY_BEGIN()`** : Initialise le framework de test au début du `setup()`.
2. **`RUN_TEST(nom_de_la_fonction)`** : Exécute une fonction de test spécifique.
3. **`UNITY_END()`** : Termine les tests et affiche le rapport final (succès/échecs).
4. **Les Assertions (`TEST_ASSERT_...`)** : Ce sont des instructions qui vérifient si les résultats sont corrects. Si l'assertion est fausse, le test échoue.

---

## 📁 Structure du dossier `test/`

PlatformIO organise les tests par dossiers. Chaque sous-dossier de `test/` représente une **suite de tests** indépendante :

```text
test/
├── README.md                 # Ce guide
├── test_storage/             # Suite de tests pour le stockage LittleFS
│   └── test_storage.cpp
└── test_config/              # Suite de tests pour valider la configuration Config.h
    └── test_config.cpp
```

---

## ✍️ Comment créer votre propre test ? (Pas à pas)

Créons ensemble un nouveau test pour vérifier que les définitions matérielles de vos cartes sont cohérentes.

### Étape 1 : Créer le dossier et le fichier
Créez un nouveau dossier nommé `test_pins` dans `test/`, puis créez un fichier `test_pins.cpp` à l'intérieur.

### Étape 2 : Écrire le code de test
Copiez et collez le modèle suivant dans `test_pins.cpp` :

```cpp
#include <Arduino.h>
#include <unity.h>
#include "Pins.h" // Nous importons notre fichier de pins à tester

// 1. Déclarer une fonction pour chaque test individuel
void test_led_pin_valide() {
    // Nous voulons nous assurer que le pin de la LED est bien configuré sur le GPIO 2
    // car sur NodeMCU (ESP8266) et ESP32Dev, la LED intégrée est sur la broche 2.
    TEST_ASSERT_EQUAL_INT(2, PIN_LED);
}

void test_ldr_pin_non_negatif() {
    // Le capteur de luminosité (LDR) doit être branché sur une broche analogique valide (valeur positive).
    TEST_ASSERT_GREATER_OR_EQUAL_INT(0, PIN_LDR);
}

// 2. Le setup() initialise et lance les tests
void setup() {
    // Un petit délai est nécessaire pour que la communication USB Série s'établisse
    delay(2000);
    
    UNITY_BEGIN(); // Démarrer Unity
    
    // Lancer chaque test défini ci-dessus
    RUN_TEST(test_led_pin_valide);
    RUN_TEST(test_ldr_pin_non_negatif);
    
    UNITY_END(); // Terminer et afficher le bilan
}

// 3. Le loop() reste obligatoirement vide
void loop() {
}
```

---

## 🚀 Comment exécuter les tests ?

Vous pouvez lancer les tests sur votre matériel réel connecté en USB.

### Méthode 1 : Via l'interface graphique de VS Code
1. Cliquez sur l'icône **PlatformIO** (la tête d'extraterrestre sur le côté gauche).
2. Déroulez le menu de votre carte (ex : `esp32dev` ou `nodemcuv2`).
3. Cliquez sur **Test**.

### Méthode 2 : Via le Terminal de commande
Ouvrez le terminal de VS Code et tapez :

```bash
# Lancer les tests sur tous les environnements
pio test

# Lancer les tests uniquement sur l'ESP32
pio test -e esp32dev

# Lancer uniquement une suite de test spécifique (ex: test_storage)
pio test -f test_storage
```

---

## 📚 Liste des Assertions les plus utiles

Voici un résumé des outils fournis par **Unity** pour vérifier vos variables et états :

| Assertion | Rôle | Exemple d'utilisation |
| :--- | :--- | :--- |
| `TEST_ASSERT_TRUE(condition)` | Vérifie que la condition est **vraie**. | `TEST_ASSERT_TRUE(Storage::begin());` |
| `TEST_ASSERT_FALSE(condition)` | Vérifie que la condition est **fausse**. | `TEST_ASSERT_FALSE(1 == 2);` |
| `TEST_ASSERT_EQUAL_INT(attendu, reel)` | Vérifie l'égalité de deux **nombres entiers**. | `TEST_ASSERT_EQUAL_INT(2, PIN_LED);` |
| `TEST_ASSERT_EQUAL_STRING(attendu, reel)` | Vérifie l'égalité de deux **chaînes de caractères**. | `TEST_ASSERT_EQUAL_STRING("ON", state.c_str());` |
| `TEST_ASSERT_GREATER_THAN(seuil, valeur)`| Vérifie que la valeur est **supérieure** au seuil. | `TEST_ASSERT_GREATER_THAN(0, count);` |
| `TEST_ASSERT_NOT_NULL(pointeur)` | Vérifie que le pointeur n'est **pas vide/nul**. | `TEST_ASSERT_NOT_NULL(ptr);` |

Félicitations, vous avez maintenant toutes les clés pour écrire vos propres tests et sécuriser vos projets d'IoT ! 🚀
