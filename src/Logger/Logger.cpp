//
// Created by timdi on 27/01/2025.
//

#include "Logger.h"
#include "LogDatabase.h"
#include <iostream>

// Get the singleton instance of Logger
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Log a message with a given level
void Logger::log(const std::string& level, const std::string& message) {
    LogEntry entry = {getCurrentTimestamp(), message, level};
    LogDatabase::getInstance().addLog(entry);

    // Print to console (optional)
    // std::cout << "[" << level << "] " << entry.timestamp << ": " << message << std::endl;
}

// Log an informational message
void Logger::logInfo(const std::string& message) {
    log("INFO", message);
}

// Log a warning message
void Logger::logWarning(const std::string& message) {
    log("WARNING", message);
}

// Log an error message
void Logger::logError(const std::string& message) {
    log("ERROR", message);
}

// Generate a placeholder timestamp (to be replaced with actual implementation)
std::string Logger::getCurrentTimestamp() const {
    return "2025-01-27 12:00:00"; // Placeholder
}
