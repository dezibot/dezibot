#ifndef LABYRINTH_MOVEMENT_H
#define LABYRINTH_MOVEMENT_H

#include <cstdint>
#include "LabyrinthConfig.h"

#define GREEN_LEFT 0
#define RED_LEFT 1  

class LabyrinthMovement {
private:
    LabyrinthConfig& config;
    double calibratedRed, calibratedGreen, calibratedBlue;
    bool invertComparison;
    bool isOnWhite;
    int currentColorMode;

public:
    LabyrinthMovement(LabyrinthConfig& config);
    void setColorMode(int colorMode);
    int getColorMode();
    void toggleColorMode();
    void calibrateWhite();
    void deadEndRotation();
    void moveMotor(uint16_t leftSpeed, uint16_t rightSpeed, int duration);
    void moveStraight();
    void moveLeft();
    void moveRight();
    void stopMotors();
    void colorSwitch();
    void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue);
    bool compareColors(double percentageRed, double percentageGreen);
    bool isColorCloseTo(double initialValue, double newValue, double tolerance);
    void controlMotors(bool isFirstGreater);
    void setMotorSpeeds(uint16_t leftSpeed, uint16_t rightSpeed);

    bool getIsOnWhite() const;
    void setIsOnWhite(bool value);
    double getCalibratedRed() const;
    double getCalibratedGreen() const;
};

#endif