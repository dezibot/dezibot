#include <Dezibot.h>

Dezibot dezibot = Dezibot();

const double BASE_SPEED = 3900;

void setup() {
  Serial.begin(115200);
  dezibot.begin();
  dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);
}

void loop() {
  double percentageRed, percentageGreen, percentageBlue;
  getColorPercentages(percentageRed, percentageGreen, percentageBlue);

  dezibot.display.clear();
  dezibot.display.println("R: " + String(percentageRed, 2) + " G: " + String(percentageGreen, 2));
  delay(100);

  if (percentageRed > percentageGreen) {
    displayMessage("ROT > GRÜN: Rechts drehen.");
    dezibot.motion.left.setSpeed(BASE_SPEED);
    dezibot.motion.right.setSpeed(2500); // Nach rechts drehen
  } else {
    displayMessage("GRÜN >= ROT: Links drehen.");
    dezibot.motion.left.setSpeed(2500); // Nach links drehen
    dezibot.motion.right.setSpeed(BASE_SPEED);
  }
}

void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) {
  uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED);
  uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
  uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
  double sumColor = red + green + blue;

  percentageRed = (red / sumColor) * 100.0;
  percentageGreen = (green / sumColor) * 100.0;
  percentageBlue = (blue / sumColor) * 100.0;
}

void displayMessage(const char *message) {
  dezibot.display.clear();
  dezibot.display.println(message);
  Serial.println(message);
}
