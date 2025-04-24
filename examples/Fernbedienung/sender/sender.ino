#include "Dezibot.h"


Dezibot dezibot = Dezibot();

void setup() {
  dezibot.begin();
  dezibot.communication.begin();
  dezibot.communication.setGroupNumber(5);
}

void loop() {
  switch (dezibot.motion.detection.getTiltDirection()) {
    case Front:
      dezibot.multiColorLight.setTopLeds(GREEN);
      dezibot.communication.sendMessage("vorn");
      break;
    case Left:
      dezibot.multiColorLight.setTopLeds(YELLOW);
      dezibot.communication.sendMessage("links");
      break;
    case Right:
      dezibot.multiColorLight.setTopLeds(TURQUOISE);
      dezibot.communication.sendMessage("rechts");
      break;
    case Back:
      dezibot.multiColorLight.setTopLeds(BLUE);
      dezibot.communication.sendMessage("stop");
      break;
    case Flipped:
      dezibot.multiColorLight.setTopLeds(PINK);
      dezibot.communication.sendMessage("stop");
      break;
    case Neutral:
      dezibot.multiColorLight.turnOffLed();
      dezibot.communication.sendMessage("stop");
      break;

    case Error:
      dezibot.multiColorLight.setTopLeds(RED);
      dezibot.communication.sendMessage("stop");
      break;
  }

  delay(100);
}
