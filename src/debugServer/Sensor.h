/**
 * @file Sensor.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements sensors as objects.
 * Each Sensor object has a name, a class name and a list of SensorFunction objects.
 * This is utilized to dynamically add sensors and their functions to the debug server.
 * Webpages for Settings and LiveData are generated based on the Sensor objects.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "SensorFunction.h"
#include <string>
#include <vector>

class Sensor {
private:
    std::string sensorName;
    std::string className;
    std::vector<SensorFunction> sensorFunctions;

public:
    explicit Sensor(const std::string& name, const std::string& className);

    /**
     * @brief Add a SensorFunction object to the Sensor object.
     * @param function
     * @return void
     */
    void addFunction(SensorFunction& function);

    /**
     * @brief Get the class name of the sensor.
     * @return std::string
     */
    std::vector<SensorFunction>& getSensorFunctions();

    /**
     * @brief Get the name of the sensor.
     * @return std::string
     */
    std::string& getSensorName();
};

#endif //SENSOR_H
