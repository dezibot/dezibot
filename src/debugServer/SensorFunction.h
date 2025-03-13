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
    explicit SensorFunction(const std::string& name, std::function<void()> function);
    void setSensorState(bool state);
    bool getSensorState();
    std::string& getFunctionName();
};



#endif //SENSORFUNCTIONS_H
