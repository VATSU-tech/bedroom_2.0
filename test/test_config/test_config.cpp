#include <Arduino.h>
#include <unity.h>
#include "Config.h"

/**
 * @brief Vérifie que la configuration des réseaux Wi-Fi est valide.
 * S'assure que le tableau de réseaux n'est pas vide et que chaque réseau a un SSID et un mot de passe valides.
 */
void test_wifi_networks_configured() {
    // Ce test s'applique si la liste des réseaux contient des éléments
    TEST_ASSERT_GREATER_THAN(0, WIFI_NETWORKS_COUNT);
    
    for (size_t i = 0; i < WIFI_NETWORKS_COUNT; i++) {
        // Le SSID ne doit pas être vide
        TEST_ASSERT_NOT_NULL(WIFI_NETWORKS[i].ssid);
        TEST_ASSERT_GREATER_THAN(0, strlen(WIFI_NETWORKS[i].ssid));
        
        // Le mot de passe ne doit pas être vide
        TEST_ASSERT_NOT_NULL(WIFI_NETWORKS[i].password);
        TEST_ASSERT_GREATER_THAN(0, strlen(WIFI_NETWORKS[i].password));
    }
}

/**
 * @brief Vérifie la validité des identifiants du Point d'Accès (Hotspot).
 * S'assure que le SSID n'est pas vide et que le mot de passe respecte la longueur minimale WPA2 de 8 caractères.
 */
void test_ap_credentials() {
    // Vérification du SSID de l'AP
    TEST_ASSERT_NOT_NULL(AP_SSID);
    TEST_ASSERT_GREATER_THAN(0, strlen(AP_SSID));
    
    // Vérification de la sécurité minimale WPA2 (8 caractères pour la clé)
    TEST_ASSERT_NOT_NULL(AP_PASSWORD);
    TEST_ASSERT_GREATER_OR_EQUAL(8, strlen(AP_PASSWORD));
}

/**
 * @brief Vérifie que le nom de domaine local est correctement configuré.
 * S'assure que le domaine se termine bien par le suffixe standardisé ".local".
 */
void test_local_domain() {
    String domain = LOCAL_DOMAIN;
    // Le domaine doit se terminer par ".local"
    TEST_ASSERT_TRUE(domain.endsWith(".local"));
}

void setup() {
    // Petit délai d'attente pour la liaison série
    delay(2000);
    
    UNITY_BEGIN();
    
    RUN_TEST(test_wifi_networks_configured);
    RUN_TEST(test_ap_credentials);
    RUN_TEST(test_local_domain);
    
    UNITY_END();
}

void loop() {
    // Requis par Arduino
}
