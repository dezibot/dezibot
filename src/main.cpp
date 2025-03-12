#include <Arduino.h>
#include <Dezibot.h>
#include <cstdlib>
#include <vector>
#include <logger/LogDatabase.h>
#include <logger/Logger.h>

Dezibot dezibot;

void generateRandomLog() {
    static const std::vector<std::string> levels = {"INFO", "WARNING", "ERROR"};
    static const std::vector<std::string> messages = {
        "System initialized successfully.",
        "Low memory warning.",
        "Failed to connect to the server.",
        "File not found.",
        "User login successful.",
        "Disk space is critically low.",
        "Unhandled exception occurred.",
        "Configuration loaded."
    };

    // Generate a random level, message, and timestamp
    const std::string& message = messages[std::rand() % messages.size()];

    // Log the random entry
    Logger::getInstance().logInfo(message);
}

void processAllLogs() {
    const auto& logs = LogDatabase::getInstance().getLogs();

    for (const auto& log : logs) {
        if (log.level != LogEntry::Level::DEBUG) {
            // Construct a string representation of the log entry
            std::string logEntry = "[" + std::to_string(log.level) + "] " + log.timestamp + ": " + log.message;

            // Call the placeholder function with the constructed log string
            dezibot.display.println(logEntry.c_str());
        }
    }
}

void setup() {
    dezibot.begin();
}

void loop() {
    dezibot.colorDetection.getAmbientLight();

    dezibot.motion.move(100);
    dezibot.motion.stop();
    // generateRandomLog();
    processAllLogs();

    // const String test = "Hello World!";
    // dezibot.display.println(test);
    delay(5000);
}

