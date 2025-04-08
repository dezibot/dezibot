#include "Dezibot.h"

Dezibot dezibot = Dezibot();
void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
  dezibot.multiColorLight.turnOffLed();
  }

void loop() {
  // put your main code here, to run repeatedly:
  dezibot.display.clear();
  int light = dezibot.lightDetection.getValue(DL_FRONT);
  dezibot.display.print(light);
  
  if(light > 2000) {
    dezibot.motion.move();
  } else {
    dezibot.motion.rotateClockwise();
  }
  delay(100);

}
