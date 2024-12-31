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
#include "labyrinthSolver/movement/LabyrinthConfig.h"
#include "labyrinthSolver/movement/LabyrinthMovement.h"

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
    LabyrinthConfig labyrinthConfig;
    LabyrinthMovement labyrinthMovement;
    void begin(void);
};

#endif //Dezibot_h