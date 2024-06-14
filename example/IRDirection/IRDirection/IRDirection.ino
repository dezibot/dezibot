#include "Dezibot.h"

Dezibot dezibot =  Dezibot();
void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Front:");
  Serial.print(dezibot.lightDetection.getAverageValue(IR_FRONT,20,1));
  Serial.print(",Left:");
  Serial.print(dezibot.lightDetection.getAverageValue(IR_LEFT,20,1));
  Serial.print(",Right:");
  Serial.print(dezibot.lightDetection.getAverageValue(IR_RIGHT,20,1));
  Serial.print(",Back:");
  Serial.println(dezibot.lightDetection.getAverageValue(IR_BACK,20,1));
  switch(dezibot.lightDetection.getBrightest(IR)){
    case IR_FRONT:
      dezibot.multiColorLight.setTopLeds(BLUE);
      break;
    case IR_LEFT:
      dezibot.multiColorLight.setTopLeds(RED);
      break;
    case IR_RIGHT:
      dezibot.multiColorLight.setTopLeds(GREEN);
      break;
    case IR_BACK:
      dezibot.multiColorLight.setTopLeds(YELLOW);
      break;
  }
  //delay(100);
}
