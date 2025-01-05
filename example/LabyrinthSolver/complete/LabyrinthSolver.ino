#include <Dezibot.h>

LabyrinthConfig config;
Dezibot dezibot = Dezibot();

void setup() {
    Serial.begin(115200);
    dezibot.begin();
    dezibot.multiColorLight.setLed(BOTTOM, 88, 100, 58);
    
    dezibot.labyrinthSolver.start();
}

void loop() {
}