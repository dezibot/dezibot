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

enum CrossingType {
    CROSSING_T1,
    CROSSING_T2,
    CROSSING_T3,
    CROSSING_T,
    CROSSING_X,
    CROSSING_UNKNOWN
};

#endif