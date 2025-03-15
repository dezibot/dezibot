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
    String htmlContent = readHtmlFromFile("/SettingsPage.html");
    serverPointer->send(200, "text/html", htmlContent);
}

// send the JSON representation of sensors and their states
void SettingsPage::sendSensorData() const {
    DynamicJsonDocument jsonDocument(4096);
    JsonArray sensorsJson = jsonDocument.to<JsonArray>();

    // iterate over sensors and their functions, add them to the JSON document
    auto& sensors = dezibot.debugServer.getSensors();
    for (auto& sensor : sensors) {
        JsonObject sensorJson = sensorsJson.createNestedObject();
        sensorJson["sensorName"] = sensor.getSensorName();

        JsonArray functionsJson = sensorJson.createNestedArray("functions");
        for (auto& sensorFunction : sensor.getSensorFunctions()) {
            JsonObject functionJson = functionsJson.createNestedObject();
            functionJson["name"] = sensorFunction.getFunctionName();
            functionJson["state"] = sensorFunction.getSensorState();
        }
    }

    // send the JSON response
    String jsonResponse;
    serializeJson(jsonDocument, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}

// toggle the sensorfunction state
void SettingsPage::toggleSensorFunction() {
    // error handling
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

    // check if the JSON contains the required keys
    if (json.containsKey("sensorFunction") && json.containsKey("enabled")) {
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
