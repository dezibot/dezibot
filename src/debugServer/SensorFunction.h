/**
 * @file SensorFunction.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements sensor functions as objects.
 * Sensorfunctions are used to store the state of a sensor and to provide the value of the sensor as string.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SENSORFUNCTIONS_H
#define SENSORFUNCTIONS_H

#include <string>

class SensorFunction {
private:
    std::string name;
    bool sensorState; // sensor enabled/disabled for live data
public:
    explicit SensorFunction(const std::string& name);

    /**
     * @brief Set the sensor state (enabled/disabled) for live data.
     * @param state enabled (true) or disabled (false)
     * @return void
     */
    void setSensorState(bool state);

    /**
     * @brief Get the sensor state (enabled/disabled) for live data.
     * @return bool
     */
    bool getSensorState();

    /**
     * @brief Get the name of the sensor function.
     * @return std::string
     */
    std::string& getFunctionName();

    /**
     * @brief Get the value of the sensor function formatted as string.
     * @return std::string
     */
    std::string& getStringValue();
};

#endif //SENSORFUNCTIONS_H
