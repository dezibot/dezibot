#include "DebugServer.h"
#include "PageProvider.h"
#include "MainPage.h"
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

WebServer server;

DebugServer::DebugServer():server(80){}

void DebugServer::setup() {
    const char* SSID = "Debug-Server";
    const char* PSK = "PW4studProj";

    const IPAddress local_ip(192,168,1,1);
    const IPAddress gateway(192,168,1,1);
    const IPAddress subnet(255,255,255,0);

    // setup as wifi accesspoint
    WiFi.softAP(SSID, PSK);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    // Initiate page objects
    mainPage = new MainPage(&server);
    loggingPage = new LoggingPage(&server);
    liveDataPage = new LiveDataPage(&server);
    settingsPage = new SettingsPage(&server);

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

void DebugServer::refreshPage() {
    // call periodically to handle client requests
    server.handleClient();
}

