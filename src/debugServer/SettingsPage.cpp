/**
 * @file SettingsPage.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the SettingsPage class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "SettingsPage.h"
#include "Dezibot.h"

extern Dezibot dezibot;

SettingsPage::SettingsPage(WebServer* server):serverPointer(server) {
    // Endpoint for receiving JSON representation of sensors and their states
    serverPointer->on("/settings/getSensorData", [this]() {
        sendSensorData();
    });

    // Endpoint for toggling sensor functions
    serverPointer->on("/settings/toggleFunction", HTTP_POST, [this]() {
        toggleSensorFunction();
    });
}

// send the html content of the SettingsPage
void SettingsPage::handler() {
    serveFileFromSpiffs(serverPointer, "/settingsPage.html", "text/html");
};

void SettingsPage::jsHandler() {
    serveFileFromSpiffs(serverPointer, "/js/settingsPageScript.js", "text/javascript");
}

void SettingsPage::cssHandler() {
    serveFileFromSpiffs(serverPointer, "/css/settingsPageStyle.css", "text/css");
}

// send the JSON representation of sensors and their states
void SettingsPage::sendSensorData() const {
    JsonDocument jsonDocument;
    JsonArray sensorsJson = jsonDocument.to<JsonArray>();

    // iterate over sensors and their functions, add them to the JSON document
    auto& sensors = dezibot.debugServer.getSensors();
    for (auto& sensor : sensors) {
        JsonObject sensorJson = sensorsJson.add<JsonObject>();
        sensorJson["sensorName"] = sensor.getSensorName();

        JsonArray functionsJson = sensorJson["functions"].to<JsonArray>();
        for (auto& sensorFunction : sensor.getSensorFunctions()) {
            JsonObject functionJson = functionsJson.add<JsonObject>();
            functionJson["name"] = sensorFunction.getFunctionName();
            functionJson["state"] = sensorFunction.getSensorState();
        }
    }

    // send the JSON response
    String jsonResponse;
    serializeJson(jsonDocument, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}

// receive json from the client, check if the sensor function exists and toggle its state
void SettingsPage::toggleSensorFunction() {
    // error handling, check if the request contains the required data
    if (!serverPointer->hasArg("plain")) {
        serverPointer->send(400, "application/json", R"({"error":"No data provided"})");
        return;
    }

    // check if the JSON is valid and parse it
    JsonDocument json;
    DeserializationError error = deserializeJson(json, serverPointer->arg("plain"));
    if (error) {
        serverPointer->send(400, "application/json", R"({"error":"Invalid JSON format"})");
        return;
    }

    // check if the JSON contains the required keys
    if (json["sensorFunction"].is<String>() && json["enabled"].is<bool>()) {
        String functionName = json["sensorFunction"].as<String>();
        bool isEnabled = json["enabled"].as<bool>();

        // Iterate over sensors and update sensorfunction state
        for (auto& sensor : dezibot.debugServer.getSensors()) {
            for (auto& sensorFunction : sensor.getSensorFunctions()) {
                if (sensorFunction.getFunctionName() == functionName.c_str()) {
                    sensorFunction.setSensorState(isEnabled);
                    serverPointer->send(200, "application/json", "{\"success\":true}");
                    return;
                }
            }
        }
        serverPointer->send(404, "application/json", R"({"error":"Sensor function not found"})");
    } else {
        serverPointer->send(400, "application/json", R"({"error":"Invalid data"})");
    }
}
