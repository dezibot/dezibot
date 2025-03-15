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