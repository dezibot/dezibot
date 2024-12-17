#include <Dezibot.h>

Dezibot dezibot = Dezibot();

const uint16_t BASE_SPEED = 3900;
const uint16_t MAX_SPEED = 8192;
const uint16_t ROTATE_SPEED = 2500;
const double ROTATE_SCALE = 1.0;
const double RED_SCALE = 1.0075;
const int ROTATE_DURATION = 4000;

bool once = true;

bool invertComparison = false;

void setup() {
  Serial.begin(115200);
  dezibot.begin();
  dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);
  colorSwitch(); //auskommentieren wenn links grün und rechts rot zum start
  deadEndRotation();  
}

void loop() {

  double percentageRed, percentageGreen, percentageBlue;
  getColorPercentages(percentageRed, percentageGreen, percentageBlue);

  bool isFirstGreater = compareColors(percentageRed, percentageGreen);
  controlMotors(isFirstGreater);
}

/////////////////////////////////////////////////////////////////////////////////////

bool isColorCloseTo(double initialValue, double newValue, double tolerance = 0.5) {
  Serial.println("isColorCloseTo");
  return abs(initialValue - newValue) <= tolerance;
}

void deadEndRotation() {
  Serial.println("Begin Deadend Rotation");
  colorSwitch();

  dezibot.motion.left.setSpeed(0);
  dezibot.motion.right.setSpeed(MAX_SPEED);

  delay(3000);
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
    // Serial.println(String(initialGreen - newGreen),2);
    // Serial.println(String(initialGreen - newGreen),2);
    stillOnWhite = isColorCloseTo(initialRed, newRed) && isColorCloseTo(initialGreen, newGreen);
  }

  stopMotors();
  delay(3000);
  Serial.println("End Deadend Rotation");
}

void rotateLeft() {
  Serial.println("Begin rotate Left");
  dezibot.motion.left.setSpeed(ROTATE_SPEED * ROTATE_SCALE);
  dezibot.motion.right.setSpeed(BASE_SPEED);
  colorSwitch();
  delay(ROTATE_DURATION);
  stopMotors();
  Serial.println("End rotate Left");
}

void rotateRight() {
  Serial.println("Begin rotate Right");
  dezibot.motion.left.setSpeed(BASE_SPEED);
  dezibot.motion.right.setSpeed(ROTATE_SPEED * ROTATE_SCALE);
  colorSwitch();
  delay(ROTATE_DURATION);
  stopMotors();
  Serial.println("End rotate Right");
}

void stopMotors() {
  Serial.println("Begin StopMotor");
  dezibot.motion.left.setSpeed(0);
  dezibot.motion.right.setSpeed(0);
  Serial.println("End StopMotor");
}

void colorSwitch() {
  Serial.println("Begin ColorSwitch");
  invertComparison = !invertComparison;
  Serial.println("End ColorSwitch");
}

void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) {
  Serial.println("Begin getColorPercentages");
  uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) * RED_SCALE;
  uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
  uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
  double sumColor = red + green + blue;

  percentageRed = (red / sumColor) * 100.0;
  percentageGreen = (green / sumColor) * 100.0;
  percentageBlue = (blue / sumColor) * 100.0;
  Serial.println("End getColorPercentages");
}

bool compareColors(double percentageRed, double percentageGreen) {
  Serial.println("CompareColors");
  return invertComparison ? (percentageGreen > percentageRed) : (percentageRed > percentageGreen);
}

void controlMotors(bool isFirstGreater) {
  Serial.println(isFirstGreater);
  Serial.println("Begin controlMotors");
  if (isFirstGreater) {
    Serial.println("Setze Links Speed");
    dezibot.motion.left.setSpeed(BASE_SPEED);
    Serial.println("Links Speed gesetzt");
    Serial.println("Setze rechts Speed");
    dezibot.motion.right.setSpeed(ROTATE_SPEED);
    Serial.println("Rechts Speed gesetzt");
  } else {
    Serial.println("Setze Links Speed");
    dezibot.motion.left.setSpeed(ROTATE_SPEED);
    Serial.println("Links Speed gesetzt");
    Serial.println("Setze rechts Speed");
    dezibot.motion.right.setSpeed(BASE_SPEED);
    Serial.println("Rechts Speed gesetzt");
  }
  Serial.println("End controlMotors");
}

// void displayMessage(const char *message) {
//   dezibot.display.clear();
//   dezibot.display.println(message);
//   Serial.println(message);
// }