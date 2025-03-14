#include "SensorFunction.h"

SensorFunction::SensorFunction(const std::string& name) : name(name) {
    sensorState = true;
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