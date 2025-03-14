#include "Sensor.h"

Sensor::Sensor(const std::string& name, const std::string& className) : sensorName(name), className(className) {}

// add function to sensor object
void Sensor::addFunction(SensorFunction& function) {
    sensorFunctions.push_back(function);
}

// get all sensorfunctions
std::vector<SensorFunction>& Sensor::getSensorFunctions() {
    return sensorFunctions;
}

// get sensor name
std::string& Sensor::getSensorName() {
    return sensorName;
}