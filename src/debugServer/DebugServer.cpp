#include "DebugServer.h"
#include "PageProvider.h"
#include "MainPage.h"
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <SPIFFS.h>

WebServer server;

DebugServer::DebugServer():server(80)
{
    mainPage = nullptr;
    loggingPage = nullptr;
    liveDataPage = nullptr;
    settingsPage = nullptr;
    // TODO: Maybe move this to SettingsPage or create separate class for sensor settings
    sensorStates["cd_getAmbientLight"] = false;
    sensorStates["cd_getColorValue"] = false;
    sensorStates["ld_getValueIrfront"] = false;
    sensorStates["ld_getValueIrleft"] = false;
    sensorStates["ld_getValueIrright"] = false;
    sensorStates["ld_getValueIrback"] = false;
    sensorStates["ld_getValueDlbottom"] = false;
    sensorStates["ld_getValueDlfront"] = false;
    sensorStates["ld_getBrightestIr"] = false;
    sensorStates["ld_getBrightestDl"] = false;
    sensorStates["md_getAcceleration"] = false;
    sensorStates["md_getRotation"] = false;
    sensorStates["md_getTemperature"] = false;
    sensorStates["md_getWhoAmI"] = false;
    sensorStates["md_getTilt"] = false;
    sensorStates["md_getTiltDirection"] = false;
    sensorStates["m_getSpeed"] = false;
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

    server.begin();
};

void DebugServer::handleClient() {
    // call periodically to handle client requests
    server.handleClient();
}

bool DebugServer::getSensorState(const String& sensor) {
    return sensorStates[sensor];
}

void DebugServer::setSensorState(const String& sensor, bool state) {
    sensorStates[sensor] = state;
}

std::map<String, bool>& DebugServer::getSensorStates() {
    return sensorStates;
}