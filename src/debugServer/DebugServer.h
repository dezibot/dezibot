/**
 * @file DebugServer.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements a webserver for debugging purposes.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

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

    /**
     * @brief Periodically handle client requests to the webserver.
     * @param parameter
     * @return void
     */
    static void handleClientTask(void* parameter);
    std::vector<Sensor> sensors;
public:
    DebugServer();

    /**
     * @brief Set up the DebugServer.
     * Initialize all webpage and sensor objects.
     * Configure dezibot as wifi accesspoint and set ip configuration and credentials.
     * @return void
     */
    void setup();

    /**
     * @brief Start the client handler on a separate thread to run independently of main program.
     * @return void
     */
    void beginClientHandle();

    /**
     * @brief Add a sensor to the list of sensors.
     * @param sensor
     * @return void
     */
    void addSensor(const Sensor& sensor);

    /**
     * @brief Get the list of sensors.
     * @return std::vector<Sensor>&
     */
    std::vector<Sensor>& getSensors();
};

#endif //DEBUGSERVER_H
