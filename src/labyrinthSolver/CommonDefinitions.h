#ifndef COMMON_DEFINITIONS_H
#define COMMON_DEFINITIONS_H

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

#endif