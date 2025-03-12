//
// Created by timdi on 27/01/2025.
//

#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <string>

namespace LogEntry {
    enum Level {
        INFO,
        WARNING,
        ERROR,
        DEBUG,
        TRACE
    };

    struct Entry {
        Level level;
        std::string timestamp;
        std::string message;
    };
}

#endif //LOGENTRY_H
