#include "Dezibot.h"

Dezibot dezibot = Dezibot();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dezibot.begin();
}

void loop() {
  switch (dezibot.motion.detection.getTiltDirection()) {
    case Front:
      dezibot.multiColorLight.setTopLeds(GREEN);
      break;
    case Left:
      dezibot.multiColorLight.setTopLeds(YELLOW);
      break;
    case Right:
      dezibot.multiColorLight.setTopLeds(TURQUOISE);
      break;
    case Back:
      dezibot.multiColorLight.setTopLeds(BLUE);
      break;
    case Flipped:
      dezibot.multiColorLight.setTopLeds(PINK);
      break;
    case Neutral:
      dezibot.multiColorLight.turnOffLed();
      break;

    case Error:
      dezibot.multiColorLight.setTopLeds(RED);
      break;
  }
  delay(100);
}
