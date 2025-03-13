#include "SensorFunction.h"

SensorFunction::SensorFunction(const std::string& name, std::function<void()> func) : name(name), function(func) {
    sensorState = false;
}

void SensorFunction::setSensorState(bool state) {
    sensorState = state;
}

bool SensorFunction::getSensorState() {
    return sensorState;
}

std::string& SensorFunction::getFunctionName() {
    return name;
}

std::string& SensorFunction::getStringValue() {
    return name;
}