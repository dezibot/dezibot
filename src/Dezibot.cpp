
#define SDA_PIN 1
#define SCL_PIN 2

#include "Dezibot.h"
#include <Wire.h>


Dezibot::Dezibot():multiColorLight(){};

void Dezibot::begin(void) {
    Wire.begin(SDA_PIN,SCL_PIN);
    infraredLight.begin();
    lightDetection.begin();    
    motion.begin();
    lightDetection.begin();
    colorDetection.begin();
    multiColorLight.begin();
    display.begin();
};


