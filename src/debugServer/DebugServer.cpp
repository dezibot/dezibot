#include "DebugServer.h"

#include <Dezibot.h>

#include "PageProvider.h"
#include "MainPage.h"
#include <WebServer.h>
#include <WiFi.h>
#include <SPIFFS.h>

WebServer server;
extern Dezibot dezibot;

DebugServer::DebugServer():server(80)
{
    mainPage = nullptr;
    loggingPage = nullptr;
    liveDataPage = nullptr;
    settingsPage = nullptr;
    // TODO: Maybe move this to SettingsPage or create separate class for sensor settings
    sensorStates["cd_getAmbientLight"] = true;
    sensorStates["cd_getColorValueRed"] = true;
    sensorStates["cd_getColorValueGreen"] = true;
    sensorStates["cd_getColorValueBlue"] = true;
    sensorStates["cd_getColorValueWhite"] = true;
    sensorStates["ld_getValueIrfront"] = true;
    sensorStates["ld_getValueIrleft"] = true;
    sensorStates["ld_getValueIrright"] = true;
    sensorStates["ld_getValueIrback"] = true;
    sensorStates["ld_getValueDlbottom"] = true;
    sensorStates["ld_getValueDlfront"] = true;
    sensorStates["ld_getBrightestIr"] = true;
    sensorStates["ld_getBrightestDl"] = true;
    sensorStates["m_getSpeedLeft"] = true;
    sensorStates["m_getSpeedRight"] = true;
    sensorStates["md_getAcceleration"] = true;
    sensorStates["md_getRotation"] = true;
    sensorStates["md_getTemperature"] = true;
    sensorStates["md_getWhoAmI"] = true;
    sensorStates["md_getTilt"] = true;
    sensorStates["md_getTiltDirection"] = true;
}

void DebugServer::setup() {
    const char* SSID = "Debug-Server";
    const char* PSK = "PW4studProj";

    const IPAddress local_ip(192,168,1,1);
    const IPAddress gateway(192,168,1,1);
    const IPAddress subnet(255,255,255,0);

    // initalize SPIFFS for file access
    // changes in html files require "pio run -t uploadfs" or "Upload Filesystem Image" in plugin to take effect
    SPIFFS.begin();

    // setup as wifi accesspoint
    WiFi.softAP(SSID, PSK);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    // Initiate page objects
    mainPage = new MainPage(&server);
    loggingPage = new LoggingPage(&server);
    settingsPage = new SettingsPage(&server);
    liveDataPage = new LiveDataPage(&server);

    // set uri and handler for each page
    server.on("/", [this]() {
        mainPage->handler();
    });

    server.on("/logging", [this]() {
        loggingPage->handler();
    });

    server.on("/livedata", [this]() {
        liveDataPage->handler();
    });

    server.on("/settings", [this]() {
        settingsPage->handler();
    });

    // initialize color sensor
    Sensor colorSensor("Color Sensor", "ColorDetection");
    SensorFunction getAmbientLight("getAmbientLight()", [&]() { dezibot.colorDetection.getAmbientLight(); });
    SensorFunction getColorValueRed("getColorValue(RED)", [&]() { dezibot.colorDetection.getColorValue(VEML_RED); });
    SensorFunction getColorValueGreen("getColorValue(GREEN)", [&]() { dezibot.colorDetection.getColorValue(VEML_GREEN); });
    SensorFunction getColorValueBlue("getColorValue(BLUE)", [&]() { dezibot.colorDetection.getColorValue(VEML_BLUE); });
    SensorFunction getColorValueWhite("getColorValue(WHITE)", [&]() { dezibot.colorDetection.getColorValue(VEML_WHITE); });
    colorSensor.addFunction(getAmbientLight);
    colorSensor.addFunction(getColorValueRed);
    colorSensor.addFunction(getColorValueGreen);
    colorSensor.addFunction(getColorValueBlue);
    colorSensor.addFunction(getColorValueWhite);
    addSensor(colorSensor);

    // initialize light sensor
    Sensor lightSensor("Light Sensor", "LightDetection");
    SensorFunction getValueIrFront("getValue(IR_FRONT)", [&]() { LightDetection::getValue(IR_FRONT); });
    SensorFunction getValueIrLeft("getValue(IR_LEFT)", [&]() { LightDetection::getValue(IR_LEFT); });
    SensorFunction getValueIrRight("getValue(IR_RIGHT)", [&]() { LightDetection::getValue(IR_RIGHT); });
    SensorFunction getValueIrBack("getValue(IR_BACK)", [&]() { LightDetection::getValue(IR_BACK); });
    SensorFunction getValueDlBottom("getValue(DL_BOTTOM)", [&]() { LightDetection::getValue(DL_BOTTOM); });
    SensorFunction getValueDlFront("getValue(DL_FRONT)", [&]() { LightDetection::getValue(DL_FRONT); });
    lightSensor.addFunction(getValueIrFront);
    lightSensor.addFunction(getValueIrLeft);
    lightSensor.addFunction(getValueIrRight);
    lightSensor.addFunction(getValueIrBack);
    lightSensor.addFunction(getValueDlBottom);
    lightSensor.addFunction(getValueDlFront);
    addSensor(lightSensor);

    // initialize motor
    Sensor motor("Motor", "Motion");
    SensorFunction getSpeedLeft("left.getSpeed()", [&]() { Motion::left.getSpeed(); });
    SensorFunction getSpeedRight("right.getSpeed()", [&]() { Motion::left.getSpeed(); });
    motor.addFunction(getSpeedLeft);
    motor.addFunction(getSpeedRight);
    addSensor(motor);

    // initialize motion sensor
    Sensor motionSensor("Motion Sensor", "MotionDetection");
    SensorFunction getAcceleration("getAcceleration()", [&]() { Motion::detection.getAcceleration(); });
    SensorFunction getRotation("getRotation()", [&]() { Motion::detection.getRotation(); });
    SensorFunction getTemperature("getTemperature()", [&]() { Motion::detection.getTemperature(); });
    SensorFunction getWhoAmI("getWhoAmI()", [&]() { Motion::detection.getWhoAmI(); });
    SensorFunction getTilt("getTilt()", [&]() { Motion::detection.getTilt(); });
    SensorFunction getTiltDirection("getTiltDirection()", [&]() { Motion::detection.getTiltDirection(); });
    motionSensor.addFunction(getAcceleration);
    motionSensor.addFunction(getRotation);
    motionSensor.addFunction(getTemperature);
    motionSensor.addFunction(getWhoAmI);
    motionSensor.addFunction(getTilt);
    motionSensor.addFunction(getTiltDirection);
    addSensor(motionSensor);

    server.begin();
    beginClientHandle();
};

bool DebugServer::getSensorState(const String& sensor) {
    return sensorStates[sensor];
}

void DebugServer::setSensorState(const String& sensor, bool state) {
    sensorStates[sensor] = state;
}

std::map<String, bool>& DebugServer::getSensorStates() {
    return sensorStates;
}

void DebugServer::addSensor(const Sensor& sensor) {
    sensors.push_back(sensor);
}

std::vector<Sensor>& DebugServer::getSensors() {
    return sensors;
}


void DebugServer::beginClientHandle() {
    // create a FreeRTOS task to handle client requests
    xTaskCreate(
        handleClientTask,
        "DebugServerTask",
        8192,
        this,
        1,
        nullptr
    );
}

void DebugServer::handleClientTask(void* parameter) {
    DebugServer* debugServer = static_cast<DebugServer*>(parameter);

    // continuously handle client requests
    while (debugServer->serveractive) {
        debugServer->server.handleClient();
        delay(10);
    }

    // Delete task when server is no longer active
    vTaskDelete(nullptr);
}
