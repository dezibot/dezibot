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

    // add functions to map for faster access
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
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_FRONT);
    };

    sensorValueFunctions["getValue(IR_LEFT)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_LEFT);
    };

    sensorValueFunctions["getValue(IR_RIGHT)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_RIGHT);
    };

    sensorValueFunctions["getValue(IR_BACK)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_BACK);
    };

    sensorValueFunctions["getValue(DL_BOTTOM)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(DL_BOTTOM);
    };

    sensorValueFunctions["getValue(DL_FRONT)"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(DL_FRONT);
    };

    sensorValueFunctions["getBrightest(IR)"] = [this](const JsonObject& sensorObject) {
        photoTransistors result = dezibot.lightDetection.getBrightest(IR);
        sensorObject["value"] = Utility::sensorToString(result);
    };

    sensorValueFunctions["getBrightest(DAYLIGHT)"] = [this](const JsonObject& sensorObject) {
        photoTransistors result = dezibot.lightDetection.getBrightest(DAYLIGHT);
        sensorObject["value"] = Utility::sensorToString(result);
    };

    sensorValueFunctions["getAcceleration()"] = [this](const JsonObject& sensorObject) {
        IMUResult result = dezibot.motion.detection.getAcceleration();
        String resultString = "x: " + String(result.x) + ", y: " + String(result.y) + ", z: " + String(result.z);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["getRotation()"] = [this](const JsonObject& sensorObject) {
        IMUResult result = dezibot.motion.detection.getRotation();
        String resultString = "x: " + String(result.x) + ", y: " + String(result.y) + ", z: " + String(result.z);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["getTemperature()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.detection.getTemperature();
    };

    sensorValueFunctions["getWhoAmI()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.detection.getWhoAmI();
    };

    sensorValueFunctions["getTilt()"] = [this](const JsonObject& sensorObject) {
        Orientation result = dezibot.motion.detection.getTilt();
        String resultString = "x: " + String(result.xRotation) + ", y: " + String(result.yRotation);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["getTiltDirection()"] = [this](const JsonObject& sensorObject) {
        auto result = dezibot.motion.detection.getTiltDirection();
        sensorObject["value"] = Utility::directionToString(result);
    };

    sensorValueFunctions["left.getSpeed()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.left.getSpeed();
    };

    sensorValueFunctions["right.getSpeed()"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.right.getSpeed();
    };
}

void LiveDataPage::handler() {
    String htmlContent = readHtmlFromFile("/LiveDataPage.html");
    serverPointer->send(200, "text/html", htmlContent);
};

void LiveDataPage::getEnabledSensorValues() {
    DynamicJsonDocument jsonDoc(4096);
    JsonArray sensorArray = jsonDoc.to<JsonArray>();

    Logger::getInstance().setLoggingEnabled(false);

    auto& sensors = dezibot.debugServer.getSensors();
    for (auto& sensor : sensors) {
        for (auto& sensorFunction : sensor.getSensorFunctions()) {
            if (sensorFunction.getSensorState()) { // If this function is active
                JsonObject sensorJson = sensorArray.createNestedObject();

                sensorJson["name"] = sensorFunction.getFunctionName();
                sensorValueFunctions[sensorFunction.getFunctionName()](sensorJson);
            }
        }
    }

    Logger::getInstance().setLoggingEnabled(true);


    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}