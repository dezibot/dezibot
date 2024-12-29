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
enum CrossingTypeT {
    CROSSING_T1,
    CROSSING_T2,
    CROSSING_T3,
    CROSSING_UNKNOWN
};

enum CrossingType {
    CROSSING_T,
    CROSSING_X
};

#endif