#include "LiveDataPage.h"
#include <Dezibot.h>
#include <ArduinoJson.h>
#include <logger/Logger.h>

#include "Utility.h"

extern Dezibot dezibot;

LiveDataPage::LiveDataPage(WebServer* server): serverPointer(server)
{
    server->on("/getEnabledSensorValues", [this]() {
        getEnabledSensorValues();
    });

    // add functions to map for reading sensor values
    sensorValueFunctions["getAmbientLight()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getAmbientLight();
    };

    sensorValueFunctions["getColorValue(RED)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_RED);
    };

    sensorValueFunctions["getColorValue(GREEN)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_GREEN);
    };

    sensorValueFunctions["getColorValue(BLUE)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_BLUE);
    };

    sensorValueFunctions["getColorValue(WHITE)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_WHITE);
    };

    sensorValueFunctions["getValue(IR_FRONT)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = LightDetection::getValue(IR_FRONT);
    };

    sensorValueFunctions["getValue(IR_LEFT)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = LightDetection::getValue(IR_LEFT);
    };

    sensorValueFunctions["getValue(IR_RIGHT)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = LightDetection::getValue(IR_RIGHT);
    };

    sensorValueFunctions["getValue(IR_BACK)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = LightDetection::getValue(IR_BACK);
    };

    sensorValueFunctions["getValue(DL_BOTTOM)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = LightDetection::getValue(DL_BOTTOM);
    };

    sensorValueFunctions["getValue(DL_FRONT)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = LightDetection::getValue(DL_FRONT);
    };

    sensorValueFunctions["getBrightest(IR)"] = [this](const JsonObject& sensorObject) {
        photoTransistors result = LightDetection::getBrightest(IR);
        sensorObject["value"] = Utility::sensorToString(result);
    };

    sensorValueFunctions["getBrightest(DAYLIGHT)"] = [this](const JsonObject& sensorObject) {
        photoTransistors result = LightDetection::getBrightest(DAYLIGHT);
        sensorObject["value"] = Utility::sensorToString(result);
    };

    // adds value of the acceleration in x, y and z direction to the json
    sensorValueFunctions["getAcceleration()"] = [this](const JsonObject& sensorObject) {
        IMUResult result = Motion::detection.getAcceleration();
        String resultString = "x: " + String(result.x) + ", y: " + String(result.y) + ", z: " + String(result.z);
        sensorObject["value"] = resultString;
    };

    // adds value of the rotation in x, y and z direction to the json
    sensorValueFunctions["getRotation()"] = [this](const JsonObject& sensorObject) {
        IMUResult result = Motion::detection.getRotation();
        String resultString = "x: " + String(result.x) + ", y: " + String(result.y) + ", z: " + String(result.z);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["getTemperature()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = Motion::detection.getTemperature();
    };

    sensorValueFunctions["getWhoAmI()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = Motion::detection.getWhoAmI();
    };

    // adds value of the tilt in x, y and z direction to the json
    sensorValueFunctions["getTilt()"] = [this](const JsonObject& sensorObject) {
        Orientation result = Motion::detection.getTilt();
        String resultString = "x: " + String(result.xRotation) + ", y: " + String(result.yRotation);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["getTiltDirection()"] = [this](const JsonObject& sensorObject) {
        auto result = Motion::detection.getTiltDirection();
        sensorObject["value"] = Utility::directionToString(result);
    };

    sensorValueFunctions["left.getSpeed()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = Motion::left.getSpeed();
    };

    sensorValueFunctions["right.getSpeed()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = Motion::right.getSpeed();
    };
}

// send the html content of the LiveDataPage
void LiveDataPage::handler() {
    // String htmlContent = readHtmlFromFile("/LiveDataPage.html");
    // serverPointer->send(200, "text/html", htmlContent);
    serveFileFromSpiffs(serverPointer, "/LiveDataPage.html", "text/html");
};

void LiveDataPage::canvasjsHandler() {
    serveFileFromSpiffs(serverPointer, "/canvasjs.min.js", "text/javascript");
}

void LiveDataPage::chartScriptHandler() {
    serveFileFromSpiffs(serverPointer, "/chartScript.js", "text/javascript");
}

void LiveDataPage::liveDataPageStyleHandler() {
    serveFileFromSpiffs(serverPointer, "/liveDataPageStyle.css", "text/css");
}

// read values from enabled sensors and send them as json
void LiveDataPage::getEnabledSensorValues() {
    JsonDocument jsonDoc;
    JsonArray sensorArray = jsonDoc.to<JsonArray>();

    // disable logging to prevent unwanted entries
    Logger::getInstance().setLoggingEnabled(false);

    // iterate over all sensors and sensorfunctions and add the values to the json
    auto& sensors = dezibot.debugServer.getSensors();
    for (auto& sensor : sensors) {
        for (auto& sensorFunction : sensor.getSensorFunctions()) {
            if (sensorFunction.getSensorState()) {
                // read and add value if sensorfunction is enabled
                JsonObject sensorJson = sensorArray.add<JsonObject>();
                sensorJson["name"] = sensorFunction.getFunctionName();
                sensorValueFunctions[sensorFunction.getFunctionName()](sensorJson);
            }
        }
    }

    // enable logging again
    Logger::getInstance().setLoggingEnabled(true);

    // send json response
    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}