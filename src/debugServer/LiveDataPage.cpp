#include "LiveDataPage.h"
#include <Dezibot.h>
#include <ArduinoJson.h>

extern Dezibot dezibot;

LiveDataPage::LiveDataPage(WebServer* server): serverPointer(server)
{
    server->on("/getEnabledSensorValues", [this]() {
        getEnabledSensorValues();
    });

    // add functions to map for faster access
    sensorValueFunctions["cd_getAmbientLight"] = [this](JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getAmbientLight();
    };

    sensorValueFunctions["ld_getValueDlbottom"] = [this](JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(DL_BOTTOM);
    };
    // TODO: add all sensor functions
}

void LiveDataPage::handler() {
    String htmlContent = readHtmlFromFile("/LiveDataPage.html");
    serverPointer->send(200, "text/html", htmlContent);
};

void LiveDataPage::getEnabledSensorValues()
{
    DynamicJsonDocument jsonDoc(1024);
    JsonArray sensorArray = jsonDoc.to<JsonArray>();
    auto& sensorStates = dezibot.debugServer.getSensorStates();

    for (const auto& [sensor, status] : sensorStates) {
        if (status) {
            if (sensorValueFunctions.find(sensor) != sensorValueFunctions.end()) {
                JsonObject sensorObject = sensorArray.createNestedObject();

                // format string to match function name
                int underscoreIndex = sensor.indexOf("_");
                String name = sensor.substring(underscoreIndex + 1);
                name += "()";
                sensorObject["name"] = name;

                // call function to get value
                sensorValueFunctions[sensor](sensorObject);
            }
        }
    }

    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}