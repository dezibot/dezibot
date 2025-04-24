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
  Serial.print("x: "); Serial.println(dezibot.motion.detection.getAcceleration().x);
  Serial.print("y: "); Serial.println(dezibot.motion.detection.getAcceleration().y);
  Serial.print("z: "); Serial.println(dezibot.motion.detection.getAcceleration().z);
  Serial.print("Just to make sure that thing is alive, (expact 103): "); Serial.println(dezibot.motion.detection.getWhoAmI());
  Serial.println("");

  delay(500);
}

