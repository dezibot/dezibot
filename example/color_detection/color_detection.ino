#include <Dezibot.h>

#define READ_DELAY 500

Dezibot dezibot = Dezibot();

void setup() {
  Serial.begin(115200);
  dezibot.begin();
}

void loop() {
  Serial.println("");
  dezibot.display.clear();
  
  printValue(VEML_RED, "R");
  printValue(VEML_GREEN, "G");
  printValue(VEML_BLUE, "B");
  printValue(VEML_WHITE, "W");

  delay(READ_DELAY);
}

void printValue(color color, String prefix) {
  uint16_t colorValue = dezibot.colorDetection.getColorValue(color);

  dezibot.display.print(prefix);
  dezibot.display.print(" ");
  dezibot.display.println(colorValue);

  Serial.print(prefix);
  Serial.print(" ");
  Serial.println(colorValue);
}
