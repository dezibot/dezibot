#include "LabyrinthConfig.h"

uint16_t LabyrinthConfig::getBaseSpeed() const {
    return baseSpeed;
}

uint16_t LabyrinthConfig::getMaxSpeed() const {
    return maxSpeed;
}

uint16_t LabyrinthConfig::getRotateSpeed() const {
    return rotateSpeed;
}

double LabyrinthConfig::getRedScale() const {
    return redScale;
}

double LabyrinthConfig::getGreenScale() const {
    return greenScale;
}

int LabyrinthConfig::getRotateDuration() const {
    return rotateDuration;
}

int LabyrinthConfig::getMoveStraightDuration() const {
    return moveStraightDuration;
}

double LabyrinthConfig::getWhiteTolerance() const {
    return whiteTolerance;
}

void LabyrinthConfig::setBaseSpeed(uint16_t speed) {
    baseSpeed = speed;
}

void LabyrinthConfig::setMaxSpeed(uint16_t speed) {
    maxSpeed = speed;
}

void LabyrinthConfig::setRotateSpeed(uint16_t speed) {
    rotateSpeed = speed;
}

void LabyrinthConfig::setRedScale(double scale) {
    redScale = scale;
}

void LabyrinthConfig::setGreenScale(double scale) {
    greenScale = scale;
}

void LabyrinthConfig::setRotateDuration(int duration) {
    rotateDuration = duration;
}

void LabyrinthConfig::setMoveStraightDuration(int duration) {
    moveStraightDuration = duration;
}

void LabyrinthConfig::setWhiteTolerance(double tolerance) {
    whiteTolerance = tolerance;
}