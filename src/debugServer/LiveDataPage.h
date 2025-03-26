/**
 * @file LiveDataPage.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements the livedata page for the webserver.
 * Displays current values for sensors that are enabled for live data.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef LIVEDATAPAGE_H
#define LIVEDATAPAGE_H

#include <ArduinoJson.h>
#include "PageProvider.h"
#include <map>

class LiveDataPage: public PageProvider {
private:
    WebServer* serverPointer;
    std::map<std::string, std::function<void(JsonObject&)>> sensorValueFunctions;
public:
    explicit LiveDataPage(WebServer* server);

    /**
     * @brief provides html on initial request from client
     * @return void
     */
    void handler() override;

    /**
     * @brief Read current values from enabled sensors, format them as json and send to client.
     * @return void
     */
    void getEnabledSensorValues();
};

#endif //LIVEDATAPAGE_H
