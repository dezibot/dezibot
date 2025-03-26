/**
 * @file LoggingPage.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements the logging page for the webserver.
 * Log entries are read from the logdatabase and sent to the client.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef LOGGINGPAGE_H
#define LOGGINGPAGE_H

#include "PageProvider.h"
#include "logger/LogEntry.h"
#include "logger/LogDatabase.h"

class LoggingPage : public PageProvider{
private:
    WebServer* serverPointer;

    /**
     * @brief Read all log entries from logdatabase, format as json and send to the client.
     * Converting log entries to json format and sending them to the client is done by the processLogs function.
     * @return void
     */
    void sendLogs() const;

    /**
     * @brief Read only new log entries from logdatabase, format as json and send to the client.
     * Converting log entries to json format and sending them to the client is done by the processLogs function.
     * @return void
     */
    void sendNewLogs() const;

    /**
     * @brief Helper function for converting the logs to json format
     * Logs are filtered by log level.
     * @return void
     */
    void processLogs(const std::vector<LogEntry::Entry>& logs, const String& logLevel) const;

public:
    explicit LoggingPage(WebServer* server);

    /**
     * @brief provides html on initial request from client
     * @return void
     */
    void handler() override;
};

#endif //LOGGINGPAGE_H
