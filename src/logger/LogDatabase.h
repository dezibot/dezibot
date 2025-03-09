//
// Created by timdi on 27/01/2025.
//

#ifndef LOGDATABASE_H
#define LOGDATABASE_H

#include "LogEntry.h"
#include <vector>
#include <mutex>

class LogDatabase {
public:
    static LogDatabase& getInstance();

    void addLog(const LogEntry& entry);
    const std::vector<LogEntry>& getLogs() const;

private:
    LogDatabase() = default;
    ~LogDatabase() = default;

    LogDatabase(const LogDatabase&) = delete;
    LogDatabase& operator=(const LogDatabase&) = delete;

    std::vector<LogEntry> logEntries_;
    mutable std::mutex mutex_;
};

#endif // LOGDATABASE_H
