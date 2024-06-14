/**
 * @file LightDetection.h
 * @author Hans Haupt (hans.haupt@dezibot.de)
 * @brief Class for Reading the values of the different Phototransistors, both IR, and DaylightSensors are supported. 
 * @version 0.1
 * @date 2024-04-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LightDetection_h
#define LightDetection_h
#include <stdint.h>
#include <Arduino.h>



enum photoTransistors{
    IR_LEFT,
    IR_RIGHT,
    IR_FRONT,
    IR_BACK,
    DL_FRONT,
    DL_BOTTOM
};

struct averageMeasurement {
    photoTransistors sensor;
    uint32_t measurementAmount;
    uint32_t timeBetween;
    uint16_t result;
    bool done;
};

enum ptType{
    IR,
    DAYLIGHT
};
static const photoTransistors allIRPTs[] = {IR_FRONT,IR_LEFT,IR_RIGHT,IR_BACK};
    static const photoTransistors allDLPTs[] = {DL_BOTTOM,DL_FRONT};


class LightDetection{
public: 
    /**
     * @brief initialize the Lightdetection Compnent, must be called before the other methods are used.  
     * 
     */
    static void begin(void);

    /**
     * @brief reads the Value of the specified sensor
     * 
     * @param sensor which sensor to read
     * @return uint the reading of the sensor. between 0-4095
     */
    static uint16_t getValue(photoTransistors sensor);

    /**
     * @brief can be used to determine which sensor is exposed to the greatest amount of light
     * Can distingish between IR and Daylight 
     * 
     * @param type select which PTTransistors to compare
     * @return photoTransistors which sensor is exposed to the greatest amount of light, if all sensor read 0, the front sensor is returned
     */
    static photoTransistors getBrightest(ptType type);

    /**
     * @brief Get the Average of multiple measurments of a single PT 
     * 
     * @param sensor Which Phototransistor should be read
     * @param measurments how many measurements should be taken
     * @param timeBetween which time should elapse between
     * @return the average of all taken meaurments
     */
    static uint32_t getAverageValue(photoTransistors sensor, uint32_t measurments, uint32_t timeBetween);
protected: 
    static const uint8_t IR_PT_FRONT_ADC = 3;
    static const uint8_t IR_PT_LEFT_ADC = 4;
    static const uint8_t IR_PT_RIGHT_ADC = 5;
    static const uint8_t IR_PT_BACK_ADC = 6;

    static const uint8_t DL_PT_FRONT_ADC = 7;
    static const uint8_t DL_PT_BOTTOM_ADC = 8;

    static const uint8_t DL_PT_ENABLE = 41;
    static const uint8_t IR_PT_ENABLE = 40;

    
    static void beginInfrared(void);
    static void beginDaylight(void);
    static uint16_t readIRPT(photoTransistors sensor);
    static uint16_t readDLPT(photoTransistors sensor); 

};
#endif //LightDetection_h