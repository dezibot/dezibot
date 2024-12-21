#include <Dezibot.h>

Dezibot dezibot = Dezibot();

const uint16_t BASE_SPEED = 3900;
const uint16_t MAX_SPEED = 8192;
const uint16_t ROTATE_SPEED = 2500;
const double ROTATE_SCALE = 1.0;
const double RED_SCALE = 1.0075;
const int ROTATE_DURATION = 4000;

double CALIBRATED_RED, CALIBRATED_GREEN, CALIBRATED_BLUE;

bool invertComparison = false;
bool isOnWhite = false;

bool isColorCloseTo(double initialValue, double newValue, double tolerance = 0.7) {
  return abs(initialValue - newValue) <= tolerance;
}

void setup() {
  Serial.begin(115200);
  dezibot.begin();
  dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);
  //colorSwitch(); //auskommentieren wenn links grün und rechts rot zum start

  // dezibot.display.println("Weiß Kal.");
  // delay(5000);
  // // Kalibrierung von weiß zur Erkennung einer Sackgasse
  // getColorPercentages(CALIBRATED_RED, CALIBRATED_GREEN, CALIBRATED_BLUE);

  // dezibot.display.println("Auf Feld");
  // delay(5000);
}

void loop() {
  double percentageRed, percentageGreen, percentageBlue;
  getColorPercentages(percentageRed, percentageGreen, percentageBlue);

  // isOnWhite = isColorCloseTo(CALIBRATED_RED, percentageRed) && isColorCloseTo(CALIBRATED_GREEN, percentageGreen);
  // if(isOnWhite){
  //   deadEndRotation();
  // }

  bool isFirstGreater = compareColors(percentageRed, percentageGreen);
  Serial.println(isFirstGreater);
  controlMotors(isFirstGreater);
}

void deadEndRotation() {
  colorSwitch();

  dezibot.motion.left.setSpeed(0);
  dezibot.motion.right.setSpeed(MAX_SPEED);

  delay(3500);
  
  dezibot.motion.left.setSpeed(MAX_SPEED);
  dezibot.motion.right.setSpeed(0);

  delay(5000);
  double initialRed, initialGreen, initialBlue;
  getColorPercentages(initialRed, initialGreen, initialBlue);

  double newRed, newGreen, newBlue;
  bool stillOnWhite = true;

  while (stillOnWhite) {
    getColorPercentages(newRed, newGreen, newBlue);
    dezibot.display.println(isColorCloseTo(initialRed, newRed));
    dezibot.display.println(isColorCloseTo(initialGreen, newGreen));
    stillOnWhite = isColorCloseTo(initialRed, newRed) && isColorCloseTo(initialGreen, newGreen);
  }

  dezibot.motion.left.setSpeed(BASE_SPEED);
  dezibot.motion.right.setSpeed(BASE_SPEED);

  delay(1500);
  
  stopMotors();
  delay(3000);
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
  Serial.println(isFirstGreater);
  if (isFirstGreater) {
    dezibot.motion.left.setSpeed(BASE_SPEED);
    dezibot.motion.right.setSpeed(ROTATE_SPEED);
  } else {
    dezibot.motion.left.setSpeed(ROTATE_SPEED);
    dezibot.motion.right.setSpeed(BASE_SPEED);
  }
}