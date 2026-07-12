/**
 * @file Storage.h
 * @brief Header for LittleFS filesystem wrapper module.
 * @details Handles mounting flash storage, reading/writing files, and listing files.
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

/**
 * @class Storage
 * @brief Encapsulates ESP32/ESP8266 LittleFS filesystem routines.
 */
class Storage {
public:
    /**
     * @brief Mounts the LittleFS filesystem partition.
     * @return true if mounted successfully, false on error.
     */
    static bool begin();

    /**
     * @brief Lists all files and sizes stored in the LittleFS partition to the serial log.
     */
    static void listFiles();

    /**
     * @brief Checks if a specific file exists.
     * @param path Full filepath (starting with slash, e.g. "/index.html").
     * @return true if file exists, false otherwise.
     */
    static bool fileExists(const String& path);

    /**
     * @brief Reads a file content completely into a string object.
     * @param path Full filepath to read.
     * @return String containing file contents, empty string on failure.
     */
    static String readFile(const String& path);

    /**
     * @brief Overwrites or creates a file with text content.
     * @param path Full filepath to write.
     * @param content String content to write.
     * @return true if successful write, false otherwise.
     */
    static bool writeFile(const String& path, const String& content);

    /**
     * @brief Retrieves the file system reference.
     * @return Reference to fs::FS instance (LittleFS).
     */
    static fs::FS& getFS();
};

#endif // STORAGE_H

