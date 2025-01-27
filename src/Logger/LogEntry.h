//
// Created by timdi on 27/01/2025.
//

#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <string>

// Represents a single log entry in the database
struct LogEntry {
    std::string timestamp;  // Log timestamp
    std::string message;    // Log message
    std::string level;      // Log level (e.g., INFO, WARNING, ERROR)
};

#endif //LOGENTRY_H
