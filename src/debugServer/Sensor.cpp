/**
 * @file Sensor.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the Sensor class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Sensor.h"

Sensor::Sensor(const std::string& name, const std::string& className) : sensorName(name), className(className) {}

void Sensor::addFunction(SensorFunction& function) {
    sensorFunctions.push_back(function);
}

std::vector<SensorFunction>& Sensor::getSensorFunctions() {
    return sensorFunctions;
}

std::string& Sensor::getSensorName() {
    return sensorName;
}