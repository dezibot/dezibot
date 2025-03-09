//
// Created by timdi on 27/01/2025.
//

#include "Logger.h"
#include "LogDatabase.h"
#include <chrono>
#include <sstream>
#include <iomanip>

// Get the singleton instance of Logger
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Log a message with a given level
void Logger::log(const LogEntry::Level level, const std::string& message) const {
    // const Logging::LogEntry newEntry = {level, getCurrentTimestamp(), message};
    const LogEntry::Entry entry = {
        .level = level,
        .timestamp = getCurrentTimestamp(),
        .message = message
    };
    LogDatabase::getInstance().addLog(entry);
}

void Logger::logInfo(const std::string& message) const {
    log(LogEntry::INFO, message);
}

void Logger::logDebug(const std::string& message) const {
    log(LogEntry::DEBUG, message);
}

void Logger::logWarning(const std::string& message) const {
    log(LogEntry::WARNING, message);
}

void Logger::logError(const std::string& message) const {
    log(LogEntry::ERROR, message);
}

// Generate a placeholder timestamp (to be replaced with actual implementation)
std::string Logger::getCurrentTimestamp() const {
    using namespace std::chrono;

    // TODO: move this towards the actual boot, not in the first timestamp
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
