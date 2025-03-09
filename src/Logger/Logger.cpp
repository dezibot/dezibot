//
// Created by timdi on 27/01/2025.
//

#include "Logger.h"
#include "LogDatabase.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

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
    using namespace std::chrono;

    static const auto startTime = steady_clock::now(); // Capture app start time
    auto now = steady_clock::now();
    auto elapsed = duration_cast<milliseconds>(now - startTime);

    // Convert to HH:MM:SS.mmm format
    int hours = static_cast<int>(elapsed.count() / (1000 * 60 * 60));
    int minutes = static_cast<int>((elapsed.count() / (1000 * 60)) % 60);
    int seconds = static_cast<int>((elapsed.count() / 1000) % 60);
    int milliseconds = static_cast<int>(elapsed.count() % 1000);

    std::ostringstream timestamp;
    timestamp << std::setfill('0') << std::setw(2) << hours << ":"
              << std::setfill('0') << std::setw(2) << minutes << ":"
              << std::setfill('0') << std::setw(2) << seconds << "."
              << std::setfill('0') << std::setw(3) << milliseconds;

    return timestamp.str();
}
