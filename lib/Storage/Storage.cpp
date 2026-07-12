/**
 * @file Storage.cpp
 * @brief Implementations of filesystem utilities.
 */

#include "Storage.h"
#include "Utilities.h"

#define MODULE_NAME "Storage"

/**
 * @brief Initializes LittleFS and logs results.
 * @details Passing `true` to `LittleFS.begin` forces formatting if mounting fails 
 *          (helpful when flashing brand new ESP32 chips).
 */
bool Storage::begin() {
#ifdef ESP32
    bool success = LittleFS.begin(true);
#elif defined(ESP8266)
    bool success = LittleFS.begin();
#endif

    if (!success) {
        Utilities::log(MODULE_NAME, "Failed to mount LittleFS filesystem.");
        return false;
    }
    Utilities::log(MODULE_NAME, "LittleFS mounted successfully.");
    listFiles();
    return true;
}

/**
 * @brief Lists all files inside LittleFS root path.
 */
void Storage::listFiles() {
    Utilities::log(MODULE_NAME, "Listing files on LittleFS:");
#ifdef ESP32
    File root = LittleFS.open("/");
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
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
        Utilities::logf(MODULE_NAME, "  File: %s, Size: %d bytes", dir.fileName().c_str(), (int)dir.fileSize());
    }
#endif
}

/**
 * @brief Check existence of a file by path.
 */
bool Storage::fileExists(const String& path) {
    return LittleFS.exists(path);
}

/**
 * @brief Read file content helper.
 */
String Storage::readFile(const String& path) {
    if (!fileExists(path)) {
        Utilities::logf(MODULE_NAME, "File not found: %s", path.c_str());
        return "";
    }
    File file = LittleFS.open(path, "r");
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
    File file = LittleFS.open(path, "w");
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
    return LittleFS;
}

