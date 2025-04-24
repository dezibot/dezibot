#include "Dezibot.h"


Dezibot dezibot = Dezibot();
void setup() {
  // put your setup code here, to run once:
dezibot.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
 for (int d = 0; d < 255; d++) {
      dezibot.multiColorLight.setLed(ALL,dezibot.multiColorLight.color(d,0,255-d));
      delay(2);
  }

  for (int d = 0; d < 255; d++) {
      dezibot.multiColorLight.setLed(ALL, dezibot.multiColorLight.color(255-d, d, 0));
      delay(2);
   }

  for (int d = 0; d < 255; d++) {
      dezibot.multiColorLight.setLed(ALL, dezibot.multiColorLight.color(0, 255-d, d));
      delay(2);
  }
}
