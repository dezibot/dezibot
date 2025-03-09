#include "LightDetection.h"
#include <limits.h>
#include <logger/Logger.h>

void LightDetection::begin(void){
    LightDetection::beginInfrared();
    LightDetection::beginDaylight();

    Logger::getInstance().logDebug("Successfully started LightDetection module");
};

uint16_t LightDetection::getValue(photoTransistors sensor){
    uint16_t value;
    switch(sensor){
        //Fall Through intended
        case IR_FRONT:
        case IR_LEFT:
        case IR_RIGHT:
        case IR_BACK:
            value = readIRPT(sensor);
            break;
        case DL_BOTTOM:
        case DL_FRONT:
            value = readDLPT(sensor);
            break;
        default:
            //currently not reachable, just if enum will be extended in the future
            value = UINT16_MAX;
            break;
    }

    Logger::getInstance().logInfo(
        "Getting LightDetection Value for sensor: "
        + std::to_string(sensor)
        + "with value: " + std::to_string(value)
    );

    return value;
};

photoTransistors LightDetection::getBrightest(ptType type){
    photoTransistors maxSensor;
    uint16_t maxReading = 0;
    uint16_t currentReading = 0;
    
    if (type == IR){
        maxSensor = IR_FRONT;
       for(const auto pt : allIRPTs){
            currentReading = LightDetection::getValue(pt);
            if (currentReading > maxReading){
                maxReading = currentReading;
                maxSensor = pt;
            }
        }
    } else {
        maxSensor = DL_FRONT;
        for(const auto pt : allDLPTs){
            currentReading = LightDetection::getValue(pt);
            if (currentReading > maxReading){
                maxReading = currentReading;
                maxSensor = pt;
            }
        }
    }
    Logger::getInstance().logInfo(
        "Getting brightest sensor for type: "
        + std::to_string(type)
        + " with result: "
        + std::to_string(maxReading)
    );

    return maxSensor;
};

uint32_t LightDetection::getAverageValue(photoTransistors sensor, uint32_t measurments, uint32_t timeBetween){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    TickType_t frequency = timeBetween / portTICK_PERIOD_MS;
    uint64_t cumulatedResult = 0; 
    for(int i = 0; i < measurments; i++){
        cumulatedResult += LightDetection::getValue(sensor);
        xTaskDelayUntil(&xLastWakeTime,frequency);
    }

    Logger::getInstance().logInfo(
        "Getting average value for sensor: "
        + std::to_string(sensor)
        + "with measurements: "
        + std::to_string(measurments)
        + "with timeBetween: "
        + std::to_string(timeBetween)
        + " with result: "
        + std::to_string(cumulatedResult)
    );

    return cumulatedResult/measurments;
};

void LightDetection::beginInfrared(void){
    digitalWrite(IR_PT_ENABLE,true);
    pinMode(IR_PT_ENABLE, OUTPUT);
    pinMode(IR_PT_FRONT_ADC, INPUT);
    pinMode(IR_PT_LEFT_ADC, INPUT);
    pinMode(IR_PT_RIGHT_ADC, INPUT);
    pinMode(IR_PT_BACK_ADC, INPUT);
};

void LightDetection::beginDaylight(void){
    digitalWrite(DL_PT_ENABLE,true);
    pinMode(DL_PT_ENABLE, OUTPUT);
    pinMode(DL_PT_BOTTOM_ADC, INPUT);
    pinMode(DL_PT_FRONT_ADC, INPUT );
};

uint16_t LightDetection::readIRPT(photoTransistors sensor){
    //digitalWrite(IR_PT_ENABLE,HIGH);
    uint16_t result = 0;
    switch (sensor)
    {
    case IR_FRONT:
        result = analogRead(IR_PT_FRONT_ADC);
        break;
    case IR_LEFT:
        result = analogRead(IR_PT_LEFT_ADC);
        break;
    case IR_RIGHT:
        result = analogRead(IR_PT_RIGHT_ADC);
        break;
    case IR_BACK:
        result = analogRead(IR_PT_BACK_ADC);
        break;
    default:
        break;
    }
    //digitalWrite(IR_PT_ENABLE,LOW);
    return result;
};

uint16_t LightDetection::readDLPT(photoTransistors sensor){
    digitalWrite(DL_PT_ENABLE,HIGH);
    uint16_t result = 0;
    switch (sensor)
    {
    case DL_FRONT:
        result = analogRead(DL_PT_FRONT_ADC);
        break;
    case DL_BOTTOM:
        result = analogRead(DL_PT_BOTTOM_ADC);
        break;
    default:
        break;
    }
    digitalWrite(DL_PT_ENABLE,LOW);
    return result;
};