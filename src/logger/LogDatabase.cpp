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
const std::vector<LogEntry::Entry>& LogDatabase::getLogs() {
    std::lock_guard<std::mutex> lock(mutex_);
    // Mark all logs as "sent"
    lastSentIndex_ = logEntries_.size();
    return logEntries_;
}

std::vector<LogEntry::Entry> LogDatabase::getNewLogs() {
    std::lock_guard<std::mutex> lock(mutex_);
    const size_t currentSize = logEntries_.size();

    // no new entries
    if (lastSentIndex_ >= currentSize) {
        return {};
    }

    // Safe conversion with static_cast
    const auto diffTypeIndex = static_cast<std::vector<LogEntry::Entry>::difference_type>(lastSentIndex_);

    std::vector<LogEntry::Entry> newEntries(
        // starts from 0 + the last sent index
        logEntries_.begin() + diffTypeIndex,
        logEntries_.end()
    );

    lastSentIndex_ = currentSize;
    return newEntries;
}