#include <Dezibot.h>
#include <Wire.h>
#include <veml6040.h>

Dezibot dezibot = Dezibot();
VEML6040 rgbwSensor;

const double MAX_COLOR_VALUE = 65536;

double TARGET_RED;
double TARGET_GREEN;
double TARGET_BLUE;
const double THRESHOLD_RED = 0.5;
const double THRESHOLD_GREEN = 0.5;
const double THRESHOLD_BLUE = 0.5;

double initialYellowValue;

void setup() {
  Serial.begin(9600);
  Serial.println("Started");
  dezibot.begin();
  Serial.println("Initialised");

  if (!rgbwSensor.begin()) {
    Serial.println("ERROR: couldn't detect the sensor");
    while (1) {}
  }
  
  rgbwSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);
  dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);

  dezibot.display.print("PLACE IT UNDER COLOR RED");
  delay(10000);
  TARGET_RED = getRawColorValue(VEML_RED);

  dezibot.display.print("PLACE IT UNDER COLOR GREEN");
  delay(10000);
  TARGET_GREEN = getRawColorValue(VEML_GREEN);

  dezibot.display.print("PLACE IT UNDER COLOR BLUE");
  delay(10000);
  TARGET_BLUE = getRawColorValue(VEML_BLUE);

  double sumColor = TARGET_RED + TARGET_GREEN + TARGET_BLUE;
  double percentageRed = (TARGET_RED / sumColor) * 100;
  double percentageGreen = (TARGET_GREEN / sumColor) * 100;
  double percentageBlue = (TARGET_BLUE / sumColor) * 100;

  printValue(percentageRed, "R");
  printValue(percentageGreen, "G");
  printValue(percentageBlue, "B");
  delay(10000);
}

void loop() {
  // dezibot.motion.move(0);
  double red = getRawColorValue(VEML_RED);
  double green = getRawColorValue(VEML_GREEN);
  double blue = getRawColorValue(VEML_BLUE);
  double sumColor = red + green + blue;

  double percentageRed = (red / sumColor) * 100;
  double percentageGreen = (green / sumColor) * 100;
  double percentageBlue = (blue / sumColor) * 100;

  printValue(red, "R");
  printValue(green, "G");
  printValue(blue, "B");

  printValue(percentageRed, "R");
  printValue(percentageGreen, "G");
  printValue(percentageBlue, "B");

  // if(percentageRed <= ... && percentageBlue   >=)

  // dezibot.display.print(isBlackLine(percentageRed, percentageGreen, percentageBlue));

  // // if (!isBlackLine(percentageRed, percentageGreen, percentageBlue)) {
  // //   dezibot.motion.rotateAntiClockwise(3000);
  // //   delay(3000);
  // //   if(!isBlackLine(percentageRed, percentageGreen, percentageBlue)){
  // //     dezibot.motion.rotateClockwise(3000);
  // //     delay(3000);
  // //   }
  // // }

  // // dezibot.motion.stop();
  // delay(2000);
  // Serial.println("");
  // dezibot.display.clear();
}


bool isBlackLine(double red, double green, double blue) {
  return (abs(red - TARGET_RED) <= THRESHOLD_RED) &&
         (abs(green - TARGET_GREEN) <= THRESHOLD_GREEN) &&
         (abs(blue - TARGET_BLUE) <= THRESHOLD_BLUE);
}

double getRawColorValue(color color) {
  switch (color) {
    case VEML_RED:
      return rgbwSensor.getRed();
    case VEML_GREEN:
      return rgbwSensor.getGreen();
    case VEML_BLUE:
      return rgbwSensor.getBlue();
    case VEML_WHITE:
      return rgbwSensor.getWhite();
  }
}

void printValue(double colorValue, const char* prefix) {
  dezibot.display.print(prefix);
  dezibot.display.print(" ");
  dezibot.display.println(String(colorValue, 2));
  
  Serial.print(prefix);
  Serial.print(" ");
  Serial.println(colorValue);
}
