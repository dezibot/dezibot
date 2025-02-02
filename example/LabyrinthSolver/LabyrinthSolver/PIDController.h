#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H


#include "CommonDefinitions.h"


class PIDController {
private:
    double kp, ki, kd; // PID constants
    double previousError;
    double integral;

public:
    PIDController(double p, double i, double d);
    MotorStrength calculateMotorStrength(int red, int green, int blue);
};

#endif // PIDCONTROLLER_H
