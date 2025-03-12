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
    sensorValueFunctions["cd_getAmbientLight"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getAmbientLight();
    };

    sensorValueFunctions["cd_getColorValueRed"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_RED);
    };

    sensorValueFunctions["cd_getColorValueGreen"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_GREEN);
    };

    sensorValueFunctions["cd_getColorValueBlue"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_BLUE);
    };

    sensorValueFunctions["cd_getColorValueWhite"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.colorDetection.getColorValue(VEML_WHITE);
    };

    sensorValueFunctions["ld_getValueIrfront"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_FRONT);
    };

    sensorValueFunctions["ld_getValueIrleft"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_LEFT);
    };

    sensorValueFunctions["ld_getValueIrright"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_RIGHT);
    };

    sensorValueFunctions["ld_getValueIrback"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(IR_BACK);
    };

    sensorValueFunctions["ld_getValueDlbottom"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(DL_BOTTOM);
    };

    sensorValueFunctions["ld_getValueDlfront"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.lightDetection.getValue(DL_FRONT);
    };

    sensorValueFunctions["ld_getBrightestIr"] = [this](const JsonObject& sensorObject) {
        photoTransistors result = dezibot.lightDetection.getBrightest(IR);
        sensorObject["value"] = Utility::sensorToString(result);
    };

    sensorValueFunctions["ld_getBrightestDl"] = [this](const JsonObject& sensorObject) {
        photoTransistors result = dezibot.lightDetection.getBrightest(DAYLIGHT);
        sensorObject["value"] = Utility::sensorToString(result);
    };

    sensorValueFunctions["md_getAcceleration"] = [this](const JsonObject& sensorObject) {
        IMUResult result = dezibot.motion.detection.getAcceleration();
        String resultString = "x: " + String(result.x) + ", y: " + String(result.y) + ", z: " + String(result.z);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["md_getRotation"] = [this](const JsonObject& sensorObject) {
        IMUResult result = dezibot.motion.detection.getRotation();
        String resultString = "x: " + String(result.x) + ", y: " + String(result.y) + ", z: " + String(result.z);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["md_getTemperature"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.detection.getTemperature();
    };

    sensorValueFunctions["md_getWhoAmI"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.detection.getWhoAmI();
    };

    sensorValueFunctions["md_getTilt"] = [this](const JsonObject& sensorObject) {
        Orientation result = dezibot.motion.detection.getTilt();
        String resultString = "x: " + String(result.xRotation) + ", y: " + String(result.yRotation);
        sensorObject["value"] = resultString;
    };

    sensorValueFunctions["md_getTiltDirection"] = [this](const JsonObject& sensorObject) {
        auto result = dezibot.motion.detection.getTiltDirection();
        sensorObject["value"] = Utility::directionToString(result);
    };

    sensorValueFunctions["m_getSpeedLeft"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.left.getSpeed();
    };

    sensorValueFunctions["m_getSpeedRight"] = [this](const JsonObject& sensorObject) {
        sensorObject["value"] = dezibot.motion.right.getSpeed();
    };
}

void LiveDataPage::handler() {
    String htmlContent = readHtmlFromFile("/LiveDataPage.html");
    serverPointer->send(200, "text/html", htmlContent);
};

void LiveDataPage::getEnabledSensorValues() {
    DynamicJsonDocument jsonDoc(1024);
    JsonArray sensorArray = jsonDoc.to<JsonArray>();
    auto& sensorStates = dezibot.debugServer.getSensorStates();

    Logger::getInstance().setLoggingEnabled(false);
    for (const auto& [sensor, status] : sensorStates) {
        if (status) {
            if (sensorValueFunctions.find(sensor) != sensorValueFunctions.end()) {
                JsonObject sensorObject = sensorArray.createNestedObject();

                // format string to match function name
                sensorObject["name"] = Utility::sensorToFunction(sensor);

                // call function to get value
                sensorValueFunctions[sensor](sensorObject);
            }
        }
    }
    Logger::getInstance().setLoggingEnabled(true);

    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}