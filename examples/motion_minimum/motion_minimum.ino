#include <Dezibot.h>

Dezibot dezibot = Dezibot();
void setup() {
    dezibot.begin();   
}

void loop() {
dezibot.motion.move();
delay(1000);
dezibot.motion.rotateAntiClockwise();
delay(1000);
dezibot.motion.rotateClockwise();
delay(1000);
dezibot.motion.stop();
delay(1000);
}

