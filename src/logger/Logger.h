//
// Created by timdi on 27/01/2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static Logger& getInstance();

    void log(const std::string& level, const std::string& message);
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);

private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getCurrentTimestamp() const;
};

#endif // LOGGER_H
