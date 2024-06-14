#include "Dezibot.h"
Dezibot dezibot = Dezibot();
void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
  Serial.begin(115200);

  Serial.println("AAA");
}

//https://esp32cube.com/basis/032_millis/

int indices = 0;
void loop() {
  // put your main code here, to run repeatedly:



  int zvalue = 0; 
  for(int i = 0; i<30;i++){
    zvalue += dezibot.motion.detection.getAcceleration().z;
  }
  zvalue = zvalue/30; 

  if(zvalue < -1700){
    dezibot.multiColorLight.setLed(ALL,0x004000);
    Serial.println("AAA");
  } else if(zvalue > 1700){
    dezibot.multiColorLight.setLed(ALL,0x400000);
  } else {
    dezibot.multiColorLight.turnOffLed();
  }
}

