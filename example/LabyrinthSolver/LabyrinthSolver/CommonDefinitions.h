#ifndef COMMON_DEFINITIONS_H
#define COMMON_DEFINITIONS_H

#include <Dezibot.h>
struct MotorStrength {
    int leftMotor;
    int rightMotor;
    double error;
};

enum ColorMode {
    GREEN_LEFT,
    RED_LEFT
};

extern VEML_CONFIG ManuelConfig80;
extern VEML_CONFIG ManualConfig320;
struct PredictionData {
    float red; 
    float green;
    float blue;
    float white;
    float ambient;
    float CCT;
    float daylight;
};

enum class CrossingType {
    X,
    T1,
    T2,
    T3,
    T,
    DEAD_END
};

enum class Marker {
    Start,
    Finish,
    Crossing,
    White,
    Path
};

#endif