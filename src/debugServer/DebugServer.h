#ifndef DEBUGSERVER_H
#define DEBUGSERVER_H
#include "MainPage.h"
#include "LoggingPage.h"
#include "LiveDataPage.h"
#include "SettingsPage.h"
#include "Sensor.h"
#include "SensorFunction.h"
#include <map>
#include <thread>

class DebugServer {
private:
    WebServer server;
    MainPage* mainPage;
    LoggingPage* loggingPage;
    LiveDataPage* liveDataPage;
    SettingsPage* settingsPage;
    std::thread handleClient();
    std::map<String, bool> sensorStates; // contains state (enabled/disabled) for each sensor
    bool serveractive = true;
    static void handleClientTask(void* parameter);
    std::vector<Sensor> sensors;
public:
    DebugServer();
    void setup();
    bool getSensorState(const String& sensor);
    void setSensorState(const String& sensor, bool state);
    std::map<String, bool>& getSensorStates();
    void beginClientHandle();
    void addSensor(const Sensor& sensor);
    std::vector<Sensor>& getSensors();
};

#endif //DEBUGSERVER_H
