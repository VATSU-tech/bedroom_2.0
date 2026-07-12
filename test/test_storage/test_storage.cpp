#include <Arduino.h>
#include <unity.h>
#include "Storage.h"

/**
 * @brief Test de l'initialisation du système de fichiers LittleFS.
 * Vérifie que le montage se déroule avec succès.
 */
void test_storage_begin() {
    bool success = Storage::begin();
    TEST_ASSERT_TRUE(success);
}

/**
 * @brief Test d'écriture et de lecture d'un fichier.
 * Écrit une chaîne de caractères, s'assure qu'elle existe, la lit et vérifie
 * que le contenu récupéré est identique à celui écrit initialement.
 */
void test_file_write_read() {
    String path = "/test_unit.txt";
    String data = "Vatsu Tech Unit Test Content";
    
    // Écriture du fichier
    bool writeSuccess = Storage::writeFile(path, data);
    TEST_ASSERT_TRUE(writeSuccess);
    
    // Vérification de l'existence du fichier
    bool exists = Storage::fileExists(path);
    TEST_ASSERT_TRUE(exists);
    
    // Lecture du fichier
    String readData = Storage::readFile(path);
    TEST_ASSERT_EQUAL_STRING(data.c_str(), readData.c_str());
}

/**
 * @brief Test de lecture d'un fichier inexistant.
 * Vérifie qu'une lecture sur un chemin non valide retourne une chaîne vide.
 */
void test_file_not_found() {
    String path = "/non_existent_file.txt";
    String readData = Storage::readFile(path);
    TEST_ASSERT_EQUAL_STRING("", readData.c_str());
}

void setup() {
    // Délai pour laisser le temps au moniteur série de se connecter
    delay(2000);
    
    UNITY_BEGIN();
    
    RUN_TEST(test_storage_begin);
    RUN_TEST(test_file_write_read);
    RUN_TEST(test_file_not_found);
    
    UNITY_END();
}

void loop() {
    // Boucle vide requise par le framework Arduino
}
