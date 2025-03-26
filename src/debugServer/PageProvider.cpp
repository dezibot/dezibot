/**
 * @file PageProvider.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the PageProvider class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "PageProvider.h"
#include <Dezibot.h>
#include <logger/Logger.h>
#include "SPIFFS.h"

String PageProvider::readHtmlFromFile(const char* filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        return {"Could not read file"};
    }
    // if (!file /*|| file.isDirectory()*/) {
    //     Logger::getInstance().logError(std::string("Could not read file ") + filename);
    //     return "Could not read file " + String(filename);
    // }

    const size_t fileSize = file.size();
    String html;
    if (!html.reserve(fileSize)) {
        Logger::getInstance().logError(std::string("Could not reserve for file ") + filename);
    }; // Pre-allocate memory to improve efficiency

    constexpr size_t bufferSize = 1024;
    char buffer[bufferSize];

    while (file.available()) {
        size_t bytesRead = file.readBytes(buffer, bufferSize);
        html.concat(buffer, bytesRead);
    }

    file.close();

    // Verify the read length matches expected size
    if (html.length() != fileSize) {
        Logger::getInstance().logError(
            std::string("File read incomplete. Expected: ")
            + std::to_string(fileSize)
            + std::string(", Actual: ")
            + std::to_string(html.length())
            + std::string(" for file ")
            + filename
        );
    }

    return html;
}

void PageProvider::serveFileFromSpiffs(
    WebServer* server,
    const char* filename,
    const char* contentType
) {
    // Validate inputs
    if (!server || !filename || !contentType) {
        Logger::getInstance().logError("Invalid parameters for serveFileFromSpiffs");
        return;
    }

    File file = SPIFFS.open(filename, "r");

    // File open failure
    if (!file) {
        Logger::getInstance().logError(
            std::string("Failed to open file: ")
            + filename
        );
        server->send(500, "text/plain", "File not found");
        return;
    }

    // Directory check
    if (file.isDirectory()) {
        Logger::getInstance().logError(
            std::string("Path is directory: ")
            + filename
        );
        file.close();
        server->send(500, "text/plain", "Invalid file type");
        return;
    }

    // Get actual file size
    const size_t fileSize = file.size();

    // Stream the file
    Logger::getInstance().logTrace(
        std::string("Streaming ")
            + filename
            + " ("
            + std::to_string(fileSize)
            + " bytes)"
        );

    // For now, we always send code 200, I guess we could change this later
    const size_t bytesStreamed = server->streamFile(file, contentType, 200);
    file.close();

    // Post-stream validation
    if (bytesStreamed != fileSize) {
        Logger::getInstance().logError(
            std::string("Streaming incomplete for ")
            + filename
            + "\nExpected: "
            + std::to_string(fileSize)
            + "\nSent: "
            + std::to_string(bytesStreamed)
        );
    }
}