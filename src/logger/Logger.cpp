//
// Created by timdi on 27/01/2025.
//

#include "Logger.h"
#include "LogDatabase.h"
#include <chrono>
#include <sstream>
#include <iomanip>

std::chrono::steady_clock::time_point startTime;

// Start timer that is used for entry timestamps
void Logger::startTimer() {
	using namespace std::chrono;
	startTime = steady_clock::now();
    return;
}

// Get the singleton instance of Logger
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Log a message with a given level
void Logger::log(const LogEntry::Level level, const std::string& message) const {
    if (!loggingEnabled) {
        return;
    }
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

void Logger::logTrace(const std::string& message) const {
  	log(LogEntry::TRACE, message);
}

// Generate a placeholder timestamp (to be replaced with actual implementation)
std::string Logger::getCurrentTimestamp() const {
    using namespace std::chrono;

    auto now = steady_clock::now();
    auto elapsed = duration_cast<milliseconds>(now - startTime);

    // Create timestamp with HH:MM:SS.mm format
    int hours = static_cast<int>(elapsed.count() / (1000 * 60 * 60));
    int minutes = static_cast<int>((elapsed.count() / (1000 * 60)) % 60);
    int seconds = static_cast<int>((elapsed.count() / 1000) % 60);
    int milliseconds = static_cast<int>(elapsed.count() % 1000);

    std::ostringstream timestamp;
    timestamp << std::setfill('0') << std::setw(2) << hours << ":"
              << std::setfill('0') << std::setw(2) << minutes << ":"
              << std::setfill('0') << std::setw(2) << seconds << "."
              << std::setfill('0') << std::setw(2) << milliseconds;

    return timestamp.str();
}

void Logger::setLoggingEnabled(bool enabled) {
    loggingEnabled = enabled;
}