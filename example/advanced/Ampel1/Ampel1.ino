#include "Dezibot.h"

Dezibot dezibot = Dezibot();
void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  dezibot.multiColorLight.setLed(TOP, RED);
  delay(2000);
  dezibot.multiColorLight.setLed(TOP, YELLOW);
  delay(2000);
  dezibot.multiColorLight.setLed(TOP, GREEN);
  delay(2000);
  dezibot.multiColorLight.setLed(TOP, YELLOW);
  delay(2000);
}
