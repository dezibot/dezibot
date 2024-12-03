#include <Dezibot.h>

Dezibot dezibot = Dezibot();

const double BASE_SPEED = 3900;
const double MAX_SPEED = 8192;
const double ROTATE_SPEED = 2500;
const double RED_SCALE = 1.0075;

bool invertComparison = false;

void setup() {
  Serial.begin(115200);
  dezibot.begin();
  dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);
}
//TODO: Scale Wert für dunklen Raum --> mit perfekten Hütchen messen
void loop() {
  double percentageRed, percentageGreen, percentageBlue;
  getColorPercentages(percentageRed, percentageGreen, percentageBlue);

  bool isFirstGreater = compareColors(percentageRed, percentageGreen);
  controlMotors(isFirstGreater);
}

/////////////////////////////////////////////////////////////////////////////////////

void colorSwitch() {
  invertComparison = !invertComparison;
}

void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) {
  uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) * RED_SCALE;
  uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
  uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
  double sumColor = red + green + blue;

  percentageRed = (red / sumColor) * 100.0;
  percentageGreen = (green / sumColor) * 100.0;
  percentageBlue = (blue / sumColor) * 100.0;
}

bool compareColors(double percentageRed, double percentageGreen) {
  return invertComparison ? (percentageGreen > percentageRed) : (percentageRed > percentageGreen);
}

void controlMotors(bool isFirstGreater) {
  if (isFirstGreater) {
    dezibot.motion.left.setSpeed(BASE_SPEED);
    dezibot.motion.right.setSpeed(ROTATE_SPEED);
  } else {
    dezibot.motion.left.setSpeed(ROTATE_SPEED);
    dezibot.motion.right.setSpeed(BASE_SPEED);
  }
}

void displayMessage(const char *message) {
  dezibot.display.clear();
  dezibot.display.println(message);
  Serial.println(message);
}