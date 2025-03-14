#ifndef SENSORFUNCTIONS_H
#define SENSORFUNCTIONS_H

#include <string>
#include <functional>

class SensorFunction {
private:
    std::string name;
    std::function<void()> function;
    bool sensorState;
public:
    explicit SensorFunction(const std::string& name);
    void setSensorState(bool state);
    bool getSensorState();
    std::string& getFunctionName();
    std::string& getStringValue();
};



#endif //SENSORFUNCTIONS_H
