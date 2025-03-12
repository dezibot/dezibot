
#define SDA_PIN 1
#define SCL_PIN 2

#include "Dezibot.h"
#include <Wire.h>
#include <logger/Logger.h>


Dezibot::Dezibot(){};

void Dezibot::begin(void) {
    Wire.begin(SDA_PIN,SCL_PIN);
    // Start the timer for logging purposes
    Logger::getInstance().startTimer();
    infraredLight.begin();
    lightDetection.begin();    
    motion.begin();
    lightDetection.begin();
    colorDetection.beginAutoMode();
    multiColorLight.begin();
    display.begin();
};
