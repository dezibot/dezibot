#ifndef LABYRINTH_MOVEMENT_H
#define LABYRINTH_MOVEMENT_H

#include <cstdint>
#include "LabyrinthConfig.h"

#define GREEN_LEFT 0
#define RED_LEFT 1  

class LabyrinthMovement {
public:
    LabyrinthMovement(LabyrinthConfig& config);

    void setColorMode(int colorMode);
    int getColorMode() const;
    void toggleColorMode();
    void calibrateWhite();
    void deadEndRotation();
    void moveMotor(uint16_t leftSpeed, uint16_t rightSpeed, int duration);
    void moveStraight();
    void moveLeft();
    void moveRight();
    void stopMotors();
    void colorSwitch();
    void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) const;
    bool compareColors(double percentageRed, double percentageGreen) const;
    bool isColorCloseTo(double initialValue, double newValue, double tolerance) const;
    void controlMotors(bool isFirstGreater);
    void setMotorSpeeds(uint16_t leftSpeed, uint16_t rightSpeed);

    bool getIsOnWhite() const;
    void setIsOnWhite(bool value);

private:
    LabyrinthConfig& config;
    double calibratedRed, calibratedGreen, calibratedBlue;
    bool invertComparison;
    bool isOnWhite;
    int currentColorMode;
};

#endif // LABYRINTH_MOVEMENT_H