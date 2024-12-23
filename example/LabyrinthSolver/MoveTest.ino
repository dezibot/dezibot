#include <Dezibot.h>

class RobotConfig {
private:
  uint16_t baseSpeed = 3900;
  uint16_t maxSpeed = 8192;
  uint16_t rotateSpeed = 2500;
  double rotateScale = 1.0;
  double redScale = 1.0075;
  int rotateDuration = 4000;
  double whiteTolerance = 0.7;

public:
  uint16_t getBaseSpeed() const {
    return baseSpeed;
  }
  uint16_t getMaxSpeed() const {
    return maxSpeed;
  }
  uint16_t getRotateSpeed() const {
    return rotateSpeed;
  }
  double getRotateScale() const {
    return rotateScale;
  }
  double getRedScale() const {
    return redScale;
  }
  int getRotateDuration() const {
    return rotateDuration;
  }
  double getWhiteTolerance() const {
    return whiteTolerance;
  }

  void setBaseSpeed(uint16_t speed) {
    baseSpeed = speed;
  }
  void setMaxSpeed(uint16_t speed) {
    maxSpeed = speed;
  }
  void setRotateSpeed(uint16_t speed) {
    rotateSpeed = speed;
  }
  void setRotateScale(double scale) {
    rotateScale = scale;
  }
  void setRedScale(double scale) {
    redScale = scale;
  }
  void setRotateDuration(int duration) {
    rotateDuration = duration;
  }
  void setWhiteTolerance(double tolerance) {
    whiteTolerance = tolerance;
  }
};

RobotConfig config;
Dezibot dezibot = Dezibot();

double CALIBRATED_RED, CALIBRATED_GREEN, CALIBRATED_BLUE;
bool invertComparison = false;
bool isOnWhite = false;

enum ColorMode { RED_LEFT, GREEN_LEFT };
ColorMode currentColorMode = RED_LEFT;

// void setColorMode(ColorMode colorMode){
//   currentColorMode = colorMode;
// }

// ColorMode getColorMode() {
//   return currentColorMode;
// }

// void toggleColorMode() {
//   currentColorMode = (currentColorMode == RED_LEFT) ? GREEN_LEFT : RED_LEFT;
// }

void calibrateWhite() {
  dezibot.display.println("Weiß Kal.");
  delay(3000);
  getColorPercentages(CALIBRATED_RED, CALIBRATED_GREEN, CALIBRATED_BLUE);
  dezibot.display.println("Auf Feld");
  delay(3000);
}

void deadEndRotation() {
  colorSwitch();

  dezibot.motion.left.setSpeed(0);
  dezibot.motion.right.setSpeed(config.getMaxSpeed());

  delay(3500);

  dezibot.motion.left.setSpeed(config.getMaxSpeed());
  dezibot.motion.right.setSpeed(0);

  delay(5000);
  double initialRed, initialGreen, initialBlue;
  getColorPercentages(initialRed, initialGreen, initialBlue);

  double newRed, newGreen, newBlue;
  bool stillOnWhite = true;

  while (stillOnWhite) {
    getColorPercentages(newRed, newGreen, newBlue);
    dezibot.display.println(isColorCloseTo(initialRed, newRed, config.getWhiteTolerance()));
    dezibot.display.println(isColorCloseTo(initialGreen, newGreen, config.getWhiteTolerance()));
    stillOnWhite = isColorCloseTo(initialRed, newRed, config.getWhiteTolerance()) && isColorCloseTo(initialGreen, newGreen, config.getWhiteTolerance());
  }

  dezibot.motion.left.setSpeed(config.getBaseSpeed());
  dezibot.motion.right.setSpeed(config.getBaseSpeed());

  delay(1500);
  stopMotors();
}

void moveStraight() {
  dezibot.motion.left.setSpeed(config.getBaseSpeed());
  dezibot.motion.right.setSpeed(config.getBaseSpeed());
  colorSwitch();
  delay(config.getRotateDuration());
  stopMotors();
}

void rotateLeft() {
  dezibot.motion.left.setSpeed(config.getRotateSpeed() * config.getRotateScale());
  dezibot.motion.right.setSpeed(config.getBaseSpeed());
  colorSwitch();
  delay(config.getRotateDuration());
  stopMotors();
}

void rotateRight() {
  dezibot.motion.left.setSpeed(config.getBaseSpeed());
  dezibot.motion.right.setSpeed(config.getRotateSpeed() * config.getRotateScale());
  colorSwitch();
  delay(config.getRotateDuration());
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
  uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) * config.getRedScale();
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
  return currentColorMode == RED_LEFT ? (percentageRed > percentageGreen) : (percentageGreen > percentageRed);
}

bool isColorCloseTo(double initialValue, double newValue, double tolerance) {
  return abs(initialValue - newValue) <= tolerance;
}

void controlMotors(bool isFirstGreater) {
  Serial.println(isFirstGreater);
  if (isFirstGreater) {
    dezibot.motion.left.setSpeed(config.getBaseSpeed());
    dezibot.motion.right.setSpeed(config.getRotateSpeed());
  } else {
    dezibot.motion.left.setSpeed(config.getRotateSpeed());
    dezibot.motion.right.setSpeed(config.getBaseSpeed());
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  dezibot.begin();
  dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);
  //setColorMode(GREEN_LEFT);
  calibrateWhite();
}

void loop() {
  double percentageRed, percentageGreen, percentageBlue;
  getColorPercentages(percentageRed, percentageGreen, percentageBlue);

  isOnWhite = isColorCloseTo(CALIBRATED_RED, percentageRed, config.getWhiteTolerance()) && isColorCloseTo(CALIBRATED_GREEN, percentageGreen, config.getWhiteTolerance());
  if (isOnWhite) {
    deadEndRotation();
  }

  bool isFirstGreater = compareColors(percentageRed, percentageGreen);
  Serial.println(isFirstGreater);
  controlMotors(isFirstGreater);
}