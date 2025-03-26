/**
 * @file LoggingPage.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the LoggingPage class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "LoggingPage.h"
#include "logger/LogDatabase.h"
#include <ArduinoJson.h>
#include "Utility.h"

LoggingPage::LoggingPage(WebServer* server): serverPointer(server) {
    // Endpoint for receiving JSON representation of all logs
    serverPointer->on("/logging/getLogs", [this]() {
        sendLogs();
    });

    // Endpoint for receiving JSON representation of new logs
    serverPointer->on("/logging/getNewLogs", [this]() {
        sendNewLogs();
    });
}

// send the html content of the LoggingPage
void LoggingPage::handler() {
    String htmlContent = readHtmlFromFile("/LoggingPage.html");
    serverPointer->send(200, "text/html", htmlContent);
}

// gets logs from logger, filters them by log level and sends them as JSON
void LoggingPage::sendLogs() const {
    String logLevel = serverPointer->arg("level");
    auto& logs = LogDatabase::getInstance().getLogs();
    processLogs(logs, logLevel);
}

// gets new logs from logger, filters them by log level and sends them as JSON
void LoggingPage::sendNewLogs() const {
    String logLevel = serverPointer->arg("level");
    auto logs = LogDatabase::getInstance().getNewLogs();
    processLogs(logs, logLevel);
}

// processes logs and sends them as JSON, helper function for sendLogs and sendNewLogs
void LoggingPage::processLogs(const std::vector<LogEntry::Entry>& logs, const String& logLevel) const {
    JsonDocument jsonDocument;
    JsonArray logsJson = jsonDocument.to<JsonArray>();

    // iterate over logs and add them to the JSON document, filtering by log level
    for (const auto& log : logs) {
        if (logLevel == "ALL" || logLevel == Utility::logLevelToString(log.level)) {
            JsonObject logJson = logsJson.add<JsonObject>();
            logJson["level"] = Utility::logLevelToString(log.level);
            logJson["timestamp"] = log.timestamp;
            logJson["message"] = log.message;
        }
    }

    // send the JSON response
    String jsonResponse;
    serializeJson(jsonDocument, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}