//
// Created by timdi on 27/01/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#include "LogEntry.h"

class Logger {
public:
    void startTimer();
    static Logger& getInstance();
    void logInfo(const std::string& message) const;
    void logWarning(const std::string& message) const;
    void logError(const std::string& message) const;
    void logDebug(const std::string &message) const;
    void logTrace(const std::string& message) const;
    void setLoggingEnabled(bool enabled);

private:
    Logger() = default;
    ~Logger() = default;

    void log(LogEntry::Level level, const std::string &message) const;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getCurrentTimestamp() const;
    bool loggingEnabled = true;
};

#endif // LOGGER_H
