#ifndef DEBUGSERVER_H
#define DEBUGSERVER_H
#include "MainPage.h"
#include "LoggingPage.h"
#include "LiveDataPage.h"
#include "SettingsPage.h"
#include "Sensor.h"

class DebugServer {
private:
    WebServer server;
    MainPage* mainPage;
    LoggingPage* loggingPage;
    LiveDataPage* liveDataPage;
    SettingsPage* settingsPage;
    bool serveractive = true;
    static void handleClientTask(void* parameter);
    std::vector<Sensor> sensors;
public:
    DebugServer();
    void setup();
    void beginClientHandle();
    void addSensor(const Sensor& sensor);
    std::vector<Sensor>& getSensors();
};

#endif //DEBUGSERVER_H
