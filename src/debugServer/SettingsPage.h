/**
 * @file SettingsPage.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements the settings page for the webserver to enable/disable sensors.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "PageProvider.h"

class SettingsPage : public PageProvider {
private:
    WebServer* serverPointer;

    /**
     * @brief Sends the sensor data in json format to the client.
     * @return void
     */
    void sendSensorData() const;

    /**
     * @brief Receives json from client and enables/disables a sensor function accordingly.
     * @return void
     */
    void toggleSensorFunction();
public:
    explicit SettingsPage(WebServer* server);

    /**
     * @brief provides html on initial request from client
     * @return void
     */
    void handler() override;
};

#endif //SETTINGSPAGE_H
