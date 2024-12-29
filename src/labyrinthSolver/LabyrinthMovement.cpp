#include "LabyrinthMovement.h"
#include "../Dezibot.h"

extern Dezibot dezibot;

LabyrinthMovement::LabyrinthMovement(LabyrinthConfig& config) : config(config), invertComparison(false), isOnWhite(false), currentColorMode(GREEN_LEFT) {}

void LabyrinthMovement::setColorMode(int colorMode) {
    currentColorMode = colorMode;
}

int LabyrinthMovement::getColorMode() {
    return currentColorMode;
}

void LabyrinthMovement::toggleColorMode() {
    currentColorMode = (currentColorMode == RED_LEFT) ? GREEN_LEFT : RED_LEFT;
}

void LabyrinthMovement::calibrateWhite() {
    dezibot.display.println("Put On White");
    delay(3000);
    getColorPercentages(calibratedRed, calibratedGreen, calibratedBlue);
    dezibot.display.println("Put On Field");
    delay(3000);
}

void LabyrinthMovement::deadEndRotation() {
    colorSwitch();

    //First go left for a better turn around
    setMotorSpeeds(0, config.getMaxSpeed());

    delay(3000);

    //Then go right to turn around
    setMotorSpeeds(config.getMaxSpeed(), 0);

    //while the color is white, keep turning
    delay(5000);
    double initialRed, initialGreen, initialBlue;
    getColorPercentages(initialRed, initialGreen, initialBlue);

    double newRed, newGreen, newBlue;
    bool stillOnWhite = true;

    while (stillOnWhite) {
        getColorPercentages(newRed, newGreen, newBlue);
        stillOnWhite = isColorCloseTo(initialRed, newRed, config.getWhiteTolerance()) && isColorCloseTo(initialGreen, newGreen, config.getWhiteTolerance());
    }

    //Go straight for a bit to be in the middle of the line
    setMotorSpeeds(config.getBaseSpeed(), config.getBaseSpeed());
    delay(1000);

    stopMotors();
}

void LabyrinthMovement::moveMotor(uint16_t leftSpeed, uint16_t rightSpeed, int duration) {
    setMotorSpeeds(leftSpeed, rightSpeed);
    colorSwitch();
    delay(duration);
    stopMotors();
}

void LabyrinthMovement::moveStraight() {
    moveMotor(config.getBaseSpeed(), config.getBaseSpeed(), config.getMoveStraightDuration());
}

void LabyrinthMovement::moveLeft() {
    moveMotor(config.getRotateSpeed(), config.getBaseSpeed(), config.getRotateDuration());
}

void LabyrinthMovement::moveRight() {
    moveMotor(config.getBaseSpeed(), config.getRotateSpeed(), config.getRotateDuration());
}

void LabyrinthMovement::stopMotors() {
    setMotorSpeeds(0, 0);
}

void LabyrinthMovement::colorSwitch() {
    invertComparison = !invertComparison;
}

void LabyrinthMovement::getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) {
    uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) * config.getRedScale();
    uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN) * config.getGreenScale();
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

bool LabyrinthMovement::compareColors(double percentageRed, double percentageGreen) {
    return currentColorMode == RED_LEFT ? (percentageRed > percentageGreen) : (percentageGreen > percentageRed);
}

bool LabyrinthMovement::isColorCloseTo(double initialValue, double newValue, double tolerance) {
    return abs(initialValue - newValue) <= tolerance;
}

void LabyrinthMovement::controlMotors(bool isFirstGreater) {
    if (isFirstGreater) {
        setMotorSpeeds(config.getBaseSpeed(), config.getRotateSpeed());
    } else {
        setMotorSpeeds(config.getRotateSpeed(), config.getBaseSpeed());
    }
}

void LabyrinthMovement::setMotorSpeeds(uint16_t leftSpeed, uint16_t rightSpeed) {
    dezibot.motion.left.setSpeed(leftSpeed);
    dezibot.motion.right.setSpeed(rightSpeed);
}

bool LabyrinthMovement::getIsOnWhite() const {
    return isOnWhite;
}

void LabyrinthMovement::setIsOnWhite(bool value) {
    isOnWhite = value;
}

double LabyrinthMovement::getCalibratedRed() const {
    return calibratedRed;
}

double LabyrinthMovement::getCalibratedGreen() const {
    return calibratedGreen;
}