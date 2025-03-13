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
    void addFunction(SensorFunction& function);
    std::vector<SensorFunction>& getSensorFunctions();
    std::string& getSensorName();
};

#endif //SENSOR_H
