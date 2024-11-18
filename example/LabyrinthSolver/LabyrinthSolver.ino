#include <Dezibot.h>
#include <Wire.h>

Dezibot dezibot = Dezibot();

double TARGET_RED;
double TARGET_GREEN;
double TARGET_BLUE;
double initialYellowValue;

const double THRESHOLD_RED = 0.5;
const double THRESHOLD_GREEN = 0.5;
const double THRESHOLD_BLUE = 0.5;

void setup() {
  Serial.begin(115200);
  dezibot.begin();

  //dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);


  // dezibot.display.print("Platziere den Sensor Unter der gelben Linie");
  // delay(10000);
  // TARGET_RED = dezibot.colorDetection.getColorValue(VEML_RED);
  // TARGET_GREEN = dezibot.colorDetection.getColorValue(VEML_GREEN);
  // TARGET_BLUE = dezibot.colorDetection.getColorValue(VEML_BLUE);

  // double sumColor = TARGET_RED + TARGET_GREEN + TARGET_BLUE;
  // double percentageRed = (TARGET_RED / sumColor) * 100;
  // double percentageGreen = (TARGET_GREEN / sumColor) * 100;
  // double percentageBlue = (TARGET_BLUE / sumColor) * 100;

  // printValue(percentageRed, "R");
  // printValue(percentageGreen, "G");
  // printValue(percentageBlue, "B");
  // delay(10000);
  delay(2000);
}

void loop() {
  Serial.println("");
  dezibot.display.clear();

  // dezibot.motion.move(0);
  uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED);
  uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
  uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
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
  delay(1000);
}


// bool isBlackLine(double red, double green, double blue) {
//   return (abs(red - TARGET_RED) <= THRESHOLD_RED) &&
//          (abs(green - TARGET_GREEN) <= THRESHOLD_GREEN) &&
//          (abs(blue - TARGET_BLUE) <= THRESHOLD_BLUE);
// }

void printValue(double colorValue, String prefix) {
  dezibot.display.print(prefix);
  dezibot.display.print(" ");
  dezibot.display.println(colorValue, 2);

  Serial.print(prefix);
  Serial.print(" ");
  Serial.println(colorValue, 2);
}
