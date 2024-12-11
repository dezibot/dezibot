#include <Dezibot.h>

Dezibot dezibot = Dezibot();

const double BASE_SPEED = 3900;
const double MAX_SPEED = 8192;
const double ROTATE_SPEED = 2500;
const double ROTATE_SCALE = 1.0;
const double RED_SCALE = 1.0075;
const int ROTATE_DURATION = 4000;

bool invertComparison = false;

void setup() {
  Serial.begin(115200);
  dezibot.begin();
  dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);

  //rotateLeft();
  colorSwitch();  //auskommentieren wenn links grün und rechts rot zum start
  deadEndRotation();
}

void loop() {
  double percentageRed, percentageGreen, percentageBlue;
  getColorPercentages(percentageRed, percentageGreen, percentageBlue);

  bool isFirstGreater = compareColors(percentageRed, percentageGreen);
  Serial.println(isFirstGreater);
  controlMotors(isFirstGreater);
}

/////////////////////////////////////////////////////////////////////////////////////

bool isColorCloseTo(double initialValue, double newValue, double tolerance = 0.5) {
  return abs(initialValue - newValue) <= tolerance;
}

void deadEndRotation() {
  // dezibot.motion.left.setSpeed(0);
  // dezibot.motion.right.setSpeed(MAX_SPEED);

  // delay(2000);
  // dezibot.motion.left.setSpeed(MAX_SPEED);
  // dezibot.motion.right.setSpeed(0);

  // delay(4000);

  // double initialRed, initialGreen, initialBlue;
  // getColorPercentages(initialRed, initialGreen, initialBlue);

  // double newRed, newGreen, newBlue;
  // bool stillOnWhite = true;

  // while (stillOnWhite) {
  //   getColorPercentages(newRed, newGreen, newBlue);
  //   stillOnWhite = isColorCloseTo(initialRed, newRed) && isColorCloseTo(initialGreen, newGreen);
  // }

  // dezibot.motion.left.setSpeed(BASE_SPEED);
  // dezibot.motion.right.setSpeed(BASE_SPEED);
  // delay(700);
  // stopMotors();
  // delay(2000);
  //colorSwitch();
}

void rotateLeft() {
  dezibot.motion.left.setSpeed(ROTATE_SPEED * ROTATE_SCALE);
  dezibot.motion.right.setSpeed(BASE_SPEED);
  colorSwitch();
  delay(ROTATE_DURATION);
  stopMotors();
}

void rotateRight() {
  dezibot.motion.left.setSpeed(BASE_SPEED);
  dezibot.motion.right.setSpeed(ROTATE_SPEED * ROTATE_SCALE);
  colorSwitch();
  delay(ROTATE_DURATION);
  stopMotors();
}

void stopMotors() {
  dezibot.motion.left.setSpeed(0);
  dezibot.motion.right.setSpeed(0);
}

void colorSwitch() {
  invertComparison = !invertComparison;
}

void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) {
  uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) * RED_SCALE;
  uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
  uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
  double sumColor = red + green + blue;

   if (sumColor == 0) {
    percentageRed = percentageGreen = percentageBlue = 0;
    return;
  }

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
  delay(100);
}

void displayMessage(const char *message) {
  dezibot.display.clear();
  dezibot.display.println(message);
  Serial.println(message);
}