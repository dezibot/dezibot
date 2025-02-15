#ifndef DEBUGSERVER_H
#define DEBUGSERVER_H
#include "MainPage.h"
#include "LoggingPage.h"
#include "LiveDataPage.h"
#include "SettingsPage.h"
#include <map>

class DebugServer {
private:
    WebServer server;
    MainPage* mainPage;
    LoggingPage* loggingPage;
    LiveDataPage* liveDataPage;
    SettingsPage* settingsPage;
    std::map<String, bool> sensorStates; // contains state (enabled/disabled) for each sensor
public:
    DebugServer();
    void setup();
    void handleClient();
    bool getSensorState(const String& sensor);
    void setSensorState(const String& sensor, bool state);
    std::map<String, bool>& getSensorStates();
};

#endif //DEBUGSERVER_H
