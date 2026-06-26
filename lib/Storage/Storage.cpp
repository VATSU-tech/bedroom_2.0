#include "Storage.h"
#include "Utilities.h"

#define MODULE_NAME "Storage"

bool Storage::begin() {
    if (!SPIFFS.begin(true)) { // true allows formatting if mounting fails
        Utilities::log(MODULE_NAME, "Failed to mount SPIFFS filesystem.");
        return false;
    }
    Utilities::log(MODULE_NAME, "SPIFFS mounted successfully.");
    listFiles();
    return true;
}

void Storage::listFiles() {
    Utilities::log(MODULE_NAME, "Listing files on SPIFFS:");
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
}

bool Storage::fileExists(const String& path) {
    return SPIFFS.exists(path);
}

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

fs::FS& Storage::getFS() {
    return SPIFFS;
}
