#include <Arduino.h>
#include <Dezibot.h>
#include <cstdlib>
#include <vector>
#include <logger/LogDatabase.h>
#include <logger/Logger.h>

Dezibot dezibot;

int i = 0;

void processAllLogs() {
    const auto& logs = LogDatabase::getInstance().getLogs();

    for (const auto& log : logs) {
        if (log.level != LogEntry::Level::TRACE) {
            // Construct a string representation of the log entry
            std::string logEntry = "[" + std::to_string(log.level) + "] " + log.timestamp + ": " + log.message;

            // Call the placeholder function with the constructed log string
            dezibot.display.println(logEntry.c_str());
        }
    }
}

void setup() {
    dezibot.begin();
    dezibot.debugServer.setup();
}

void loop() {
    //dezibot.colorDetection.getAmbientLight();
    dezibot.colorDetection.getColorValue(VEML_RED);
    dezibot.display.println(i);
    i++;
    delay(30000);
}

