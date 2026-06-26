#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>

class Storage {
public:
    static bool begin();
    static void listFiles();
    static bool fileExists(const String& path);
    static String readFile(const String& path);
    static bool writeFile(const String& path, const String& content);
    static fs::FS& getFS();
};

#endif // STORAGE_H
