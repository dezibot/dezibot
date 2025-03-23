/**
 * @file DebugServer.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the DebugServer class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "DebugServer.h"
#include <Dezibot.h>

#include "PageProvider.h"
#include "MainPage.h"
#include <WebServer.h>
#include <WiFi.h>
#include <SPIFFS.h>

WebServer server;
extern Dezibot dezibot;

DebugServer::DebugServer():server(80) {
    mainPage = new MainPage(&server);
    loggingPage = new LoggingPage(&server);
    settingsPage = new SettingsPage(&server);
    liveDataPage = new LiveDataPage(&server);
}

void DebugServer::setup() {
    // set wifi credentials
    const char* SSID = "Debug-Server";
    const char* PSK = "PW4studProj";

    // set IP configuration
    const IPAddress local_ip(192,168,1,1);
    const IPAddress gateway(192,168,1,1);
    const IPAddress subnet(255,255,255,0);

    // initalize SPIFFS for file access
    // changes in html files require "pio run -t uploadfs" or "Upload Filesystem Image" in plugin to take effect
    SPIFFS.begin();

    // setup as wifi accesspoint
    WiFi.softAP(SSID, PSK);
    WiFi.softAPConfig(local_ip, gateway, subnet);

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
    SensorFunction getAmbientLight("getAmbientLight()");
    SensorFunction getColorValueRed("getColorValue(RED)");
    SensorFunction getColorValueGreen("getColorValue(GREEN)");
    SensorFunction getColorValueBlue("getColorValue(BLUE)") ;
    SensorFunction getColorValueWhite("getColorValue(WHITE)");
    colorSensor.addFunction(getAmbientLight);
    colorSensor.addFunction(getColorValueRed);
    colorSensor.addFunction(getColorValueGreen);
    colorSensor.addFunction(getColorValueBlue);
    colorSensor.addFunction(getColorValueWhite);
    addSensor(colorSensor);

    // initialize light sensor
    Sensor lightSensor("Light Sensor", "LightDetection");
    SensorFunction getValueIrFront("getValue(IR_FRONT)");
    SensorFunction getValueIrLeft("getValue(IR_LEFT)");
    SensorFunction getValueIrRight("getValue(IR_RIGHT)");
    SensorFunction getValueIrBack("getValue(IR_BACK)");
    SensorFunction getValueDlBottom("getValue(DL_BOTTOM)");
    SensorFunction getValueDlFront("getValue(DL_FRONT)");
    lightSensor.addFunction(getValueIrFront);
    lightSensor.addFunction(getValueIrLeft);
    lightSensor.addFunction(getValueIrRight);
    lightSensor.addFunction(getValueIrBack);
    lightSensor.addFunction(getValueDlBottom);
    lightSensor.addFunction(getValueDlFront);
    addSensor(lightSensor);

    // initialize motor
    Sensor motor("Motor", "Motion");
    SensorFunction getSpeedLeft("left.getSpeed()");
    SensorFunction getSpeedRight("right.getSpeed()");
    motor.addFunction(getSpeedLeft);
    motor.addFunction(getSpeedRight);
    addSensor(motor);

    // initialize motion sensor
    Sensor motionSensor("Motion Sensor", "MotionDetection");
    SensorFunction getAcceleration("getAcceleration()");
    SensorFunction getRotation("getRotation()");
    SensorFunction getTemperature("getTemperature()");
    SensorFunction getWhoAmI("getWhoAmI()");
    SensorFunction getTilt("getTilt()");
    SensorFunction getTiltDirection("getTiltDirection()");
    motionSensor.addFunction(getAcceleration);
    motionSensor.addFunction(getRotation);
    motionSensor.addFunction(getTemperature);
    motionSensor.addFunction(getWhoAmI);
    motionSensor.addFunction(getTilt);
    motionSensor.addFunction(getTiltDirection);
    addSensor(motionSensor);

    // start webserver and run client handler on separate thread
    server.begin();
    beginClientHandle();
};

void DebugServer::addSensor(const Sensor& sensor) {
    sensors.push_back(sensor);
}

std::vector<Sensor>& DebugServer::getSensors() {
    return sensors;
}

// create a FreeRTOS task to handle client requests
void DebugServer::beginClientHandle() {
    xTaskCreate(
        handleClientTask,
        "DebugServerTask",
        8192,
        this,
        1,
        nullptr
    );
}

// handle client requests
void DebugServer::handleClientTask(void* parameter) {
     auto debugServer = static_cast<DebugServer*>(parameter);

    // continuously handle client requests
    while (debugServer->serveractive) {
        debugServer->server.handleClient();
        delay(10);
    }

    // Delete task when server is no longer active
    vTaskDelete(nullptr);
}
