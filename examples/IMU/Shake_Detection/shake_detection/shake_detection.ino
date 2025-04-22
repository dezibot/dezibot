#include "Dezibot.h"

Dezibot dezibot = Dezibot();
void setup() {
  // put your setup code here, to run once:

dezibot.begin();
dezibot.multiColorLight.turnOffLed();
 
}

void loop() {
  // put your main code here, to run repeatedly:
if(dezibot.motion.detection.isShaken(1000,zAxis)){
  dezibot.multiColorLight.setTopLeds(0xFF0000);
} else if(dezibot.motion.detection.isShaken(1000,xAxis|yAxis)) {
  dezibot.multiColorLight.setTopLeds(0x00FF00);
}
}
