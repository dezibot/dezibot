//
// Created by timdi on 27/01/2025.
//

#include "LogDatabase.h"

// Get the singleton instance of LogDatabase
LogDatabase& LogDatabase::getInstance() {
    static LogDatabase instance;
    return instance;
}

// Add a log entry to the database
void LogDatabase::addLog(const LogEntry::Entry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);
    logEntries_.emplace_back(entry);
}

// Retrieve all log entries
const std::vector<LogEntry::Entry>& LogDatabase::getLogs() const {
    return logEntries_;
}