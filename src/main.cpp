#include <Arduino.h>
#include <Dezibot.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <Logger/LogDatabase.h>
#include <Logger/Logger.h>

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

    // Seed random number generator (only needed once per program)
    static bool seeded = []() { std::srand(std::time(nullptr)); return true; }();

    // Generate a random level, message, and timestamp
    std::string level = levels[std::rand() % levels.size()];
    std::string message = messages[std::rand() % messages.size()];
    std::string timestamp = "2025-01-27 12:" + std::to_string(std::rand() % 60) + ":" + std::to_string(std::rand() % 60);

    // Log the random entry
    Logger::getInstance().log(level, message);
}


void processAllLogs() {
    const auto& logs = LogDatabase::getInstance().getLogs();

    for (const auto& log : logs) {
        // Construct a string representation of the log entry
        std::string logEntry = "[" + log.level + "] " + log.timestamp + ": " + log.message;

        // Call the placeholder function with the constructed log string
        dezibot.display.println(logEntry.c_str());
    }
}

void setup() {
    dezibot.begin();
    dezibot.debugServer.setup();
}

void loop() {

    generateRandomLog();
    processAllLogs();

    // const String test = "Hello World!";
    // dezibot.display.println(test);
    dezibot.debugServer.refreshPage();
    delay(5000);
}

