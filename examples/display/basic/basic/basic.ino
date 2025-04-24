#include "Dezibot.h"

Dezibot dezibot = Dezibot();
void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  dezibot.display.print("Hello from\nDezibot!");
  delay(5000);
  dezibot.display.clear();
  dezibot.display.print("Bye!");
  delay(5000);
  dezibot.display.clear();
}
