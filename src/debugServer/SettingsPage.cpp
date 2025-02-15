#include "SettingsPage.h"
#include "Dezibot.h"

extern Dezibot dezibot;

SettingsPage::SettingsPage(WebServer* server):serverPointer(server) {}

void SettingsPage::handler() {
    if (serverPointer->method() == HTTP_GET) {
        String htmlContent = readHtmlFromFile("/SettingsPage.html");

        auto& sensorStates = dezibot.debugServer.getSensorStates();
        for (const auto& [sensor, enabled] : sensorStates) {
            String checked = enabled ? "checked" : "";
            htmlContent.replace("{{" + sensor + "}}", checked);
        }
        serverPointer->send(200, "text/html", htmlContent);

    } else if (serverPointer->method() == HTTP_POST) {
        if (serverPointer->hasArg("plain")) {
            DynamicJsonDocument json(1024);
            deserializeJson(json, serverPointer->arg("plain"));

            if (json.containsKey("sensor") && json.containsKey("enabled")) {
                String sensor = json["sensor"].as<String>();
                bool enabled = json["enabled"].as<bool>();

                auto& sensorStates = dezibot.debugServer.getSensorStates();
                if (sensorStates.find(sensor) != sensorStates.end()) {
                    sensorStates[sensor] = enabled;
                    serverPointer->send(200, "application/json", "{\"success\":true}");
                    return;
                }
            }
            serverPointer->send(400, "application/json", "{\"error\":\"Invalid data\"}");
        } else {
            serverPointer->send(400, "application/json", "{\"error\":\"No data received\"}");
        }
    } else {
        serverPointer->send(405, "text/plain", "Method not allowed");
    }
};
