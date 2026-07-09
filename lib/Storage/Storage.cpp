/**
 * @file Storage.cpp
 * @brief Implementations of filesystem utilities.
 */

#include "Storage.h"
#include "Utilities.h"

#define MODULE_NAME "Storage"

/**
 * @brief Initializes SPIFFS and logs results.
 * @details Passing `true` to `SPIFFS.begin` forces formatting if mounting fails 
 *          (helpful when flashing brand new ESP32 chips).
 */
bool Storage::begin() {
#ifdef ESP32
    bool success = SPIFFS.begin(true);
#elif defined(ESP8266)
    bool success = SPIFFS.begin();
#endif

    if (!success) {
        Utilities::log(MODULE_NAME, "Failed to mount SPIFFS filesystem.");
        return false;
    }
    Utilities::log(MODULE_NAME, "SPIFFS mounted successfully.");
    listFiles();
    return true;
}

/**
 * @brief Lists all files inside SPIFFS root path.
 */
void Storage::listFiles() {
    Utilities::log(MODULE_NAME, "Listing files on SPIFFS:");
#ifdef ESP32
    File root = SPIFFS.open("/");
    if (!root) {
        Utilities::log(MODULE_NAME, "Failed to open root directory.");
        return;
    }
    
    File file = root.openNextFile();
    while (file) {
        Utilities::logf(MODULE_NAME, "  File: %s, Size: %d bytes", file.name(), (int)file.size());
        file.close();
        file = root.openNextFile();
    }
#elif defined(ESP8266)
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
        Utilities::logf(MODULE_NAME, "  File: %s, Size: %d bytes", dir.fileName().c_str(), (int)dir.fileSize());
    }
#endif
}

/**
 * @brief Check existence of a file by path.
 */
bool Storage::fileExists(const String& path) {
    return SPIFFS.exists(path);
}

/**
 * @brief Read file content helper.
 */
String Storage::readFile(const String& path) {
    if (!fileExists(path)) {
        Utilities::logf(MODULE_NAME, "File not found: %s", path.c_str());
        return "";
    }
    File file = SPIFFS.open(path, "r");
    if (!file) {
        Utilities::logf(MODULE_NAME, "Failed to open file: %s", path.c_str());
        return "";
    }
    String content = file.readString();
    file.close();
    return content;
}

/**
 * @brief Write file content helper.
 */
bool Storage::writeFile(const String& path, const String& content) {
    File file = SPIFFS.open(path, "w");
    if (!file) {
        Utilities::logf(MODULE_NAME, "Failed to open file for writing: %s", path.c_str());
        return false;
    }
    size_t written = file.print(content);
    file.close();
    return written == content.length();
}

/**
 * @brief Return standard Filesystem reference (needed by AsyncWebServer).
 */
fs::FS& Storage::getFS() {
    return SPIFFS;
}
