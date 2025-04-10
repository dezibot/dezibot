#include "Dezibot.h"
Dezibot dezibot;
void setup() {

  dezibot.begin();
  dezibot.motion.detection.begin();
  //dezibot.motion.detection.end();
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Serial.println(dezibot.motion.detection.getTemperature());
  Serial.println(dezibot.motion.detection.getAcceleration().z);
  //Serial.println(dezibot.motion.detection.getRotation().x);
  Serial.println(dezibot.motion.detection.getWhoAmI());
  delay(5000);
}
