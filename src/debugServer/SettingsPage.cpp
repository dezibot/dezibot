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

void SettingsPage::handler() {
    String htmlContent = readHtmlFromFile("/SettingsPage.html");
    serverPointer->send(200, "text/html", htmlContent);
}

// Function 2: Sends the sensor data as JSON to the browser
void SettingsPage::sendSensorData() const {
    DynamicJsonDocument jsonDocument(4096);
    JsonArray sensorsJson = jsonDocument.to<JsonArray>();

    // Iterate over sensors managed by DebugServer
    auto& sensors = dezibot.debugServer.getSensors();
    for (auto& sensor : sensors) {
        JsonObject sensorJson = sensorsJson.createNestedObject();
        sensorJson["sensorName"] = sensor.getSensorName().c_str();

        // Populate the sensor functions
        JsonArray functionsJson = sensorJson.createNestedArray("functions");
        for (auto& sensorFunction : sensor.getSensorFunctions()) {
            JsonObject functionJson = functionsJson.createNestedObject();
            functionJson["name"] = sensorFunction.getFunctionName().c_str();
            functionJson["state"] = sensorFunction.getSensorState();
        }
    }

    // Send the JSON response
    String jsonResponse;
    serializeJson(jsonDocument, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}

// Function 3: Handles toggling of specific sensor function states
void SettingsPage::toggleSensorFunction() {
    if (!serverPointer->hasArg("plain")) {
        serverPointer->send(400, "application/json", R"({"error":"No data provided"})");
        return;
    }

    DynamicJsonDocument json(256);
    DeserializationError error = deserializeJson(json, serverPointer->arg("plain"));

    if (error) {
        serverPointer->send(400, "application/json", R"({"error":"Invalid JSON format"})");
        return;
    }

    // Extract data from JSON payload
    if (json.containsKey("sensorFunction") && json.containsKey("enabled")) {
        String functionName = json["sensorFunction"].as<String>();
        bool isEnabled = json["enabled"].as<bool>();

        // Iterate over sensors and update sensor function state
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
