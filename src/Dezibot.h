/**
 * @file Dezibot.h
 * @author Hans Haupt, Jens Wagner, Anina Morgner, Anton Jacker, Saskia Dübener
 * @brief 
 * @version 0.1
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef Dezibot_h
#define Dezibot_h

#include "motion/Motion.h"
#include "lightDetection/LightDetection.h"
#include "colorDetection/ColorDetection.h"
#include "multiColorLight/MultiColorLight.h"
#include "motionDetection/MotionDetection.h"
#include "infraredLight/InfraredLight.h"
#include "communication/Communication.h"
#include "display/Display.h"


class Dezibot {
protected:

public:
    Dezibot();
    Motion motion;
    LightDetection lightDetection;
    ColorDetection colorDetection;
    MultiColorLight multiColorLight;
    InfraredLight infraredLight;
    Communication communication;
    Display display;
    void begin(void);
};

#endif //Dezibot_h