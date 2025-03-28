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

    // default code for sending a file is 200
    const size_t bytesStreamed = server->streamFile(file, contentType, 200);
    file.close();

    // Post-stream validation, make sure that the streamed size matches the actual size
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