#include "Dezibot.h"

Dezibot dezibot =  Dezibot();
const int centeredThreshold = 50 ;

void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
  Serial.begin(115200);
}

void loop() {
  int32_t leftValue = (int32_t)dezibot.lightDetection.getAverageValue(IR_LEFT, 20, 1);
  int32_t rightValue = (int32_t)dezibot.lightDetection.getAverageValue(IR_RIGHT, 20, 1); 
  switch(dezibot.lightDetection.getBrightest(IR)){
    case IR_FRONT:
      //correct Stearing to be centered
      if( abs(leftValue-rightValue)
          < centeredThreshold){
            dezibot.motion.move();
          }else{
            if (leftValue > rightValue){
              dezibot.motion.rotateAntiClockwise();
            } else{
              dezibot.motion.rotateClockwise();
            }
          }
      dezibot.multiColorLight.setTopLeds(BLUE);
      break;
    case IR_LEFT:
      dezibot.motion.rotateAntiClockwise();
      dezibot.multiColorLight.setTopLeds(RED);
      break;
    case IR_RIGHT:
      dezibot.motion.rotateClockwise();
      dezibot.multiColorLight.setTopLeds(GREEN);
      break;
    case IR_BACK:
      if(leftValue > rightValue){
        dezibot.motion.rotateAntiClockwise();
      } else {
        dezibot.motion.rotateClockwise();
      }
      dezibot.multiColorLight.setTopLeds(YELLOW);
      break;
  }
  //delay(100);
}
