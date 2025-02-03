/**
 * @file LabyrinthMovement.h
 * @brief Header file for the LabyrinthMovement class.
 * @version 0.1
 * @date 2024-12-30
 * 
 * @author Vladislav Antonov
 * 
 * This file contains the declaration of the LabyrinthMovement class, which handles the movement logic for the labyrinth solver.
 */

#ifndef LABYRINTH_MOVEMENT_H
#define LABYRINTH_MOVEMENT_H

#include <cstdint>
#include "LabyrinthConfig.h"
#include "CommonDefinitions.h"

class LabyrinthMovement {
public:
    /**
     * @brief Constructs a new LabyrinthMovement object.
     * 
     * @param config Reference to the LabyrinthConfig object.
     */
    LabyrinthMovement(LabyrinthConfig& config);

    /**
     * @brief Sets the color mode.
     * 
     * @param colorMode The color mode to set.
     */
    void setColorMode(ColorMode colorMode);

    /**
     * @brief Gets the current color mode.
     * 
     * @return int The current color mode.
     */
    ColorMode getColorMode() const;

    /**
     * @brief Toggles the color mode between RED_LEFT and GREEN_LEFT.
     */
    void toggleColorMode();

    /**
     * @brief Calibrates the white color.
     */
    void calibrateWhite();

    /**
     * @brief Performs a dead-end rotation.
     */
    void deadEndRotation();

    /**
     * @brief Moves the motor with specified speeds and duration.
     * 
     * @param leftSpeed The speed of the left motor.
     * @param rightSpeed The speed of the right motor.
     * @param duration The duration to move the motor.
     */
    void moveMotor(uint16_t leftSpeed, uint16_t rightSpeed, int duration, bool toggleColorModeFlag = true);

    /**
     * @brief Moves the robot straight.
     */
    void moveStraight();

    /**
     * @brief Moves the robot to the left.
     */
    void moveLeft();

    /**
     * @brief Moves the robot to the right.
     */
    void moveRight();

    /**
     * @brief Stops the motors.
     */
    void stopMotors();

    /**
     * @brief Gets the color percentages.
     * 
     * @param percentageRed Reference to store the red percentage.
     * @param percentageGreen Reference to store the green percentage.
     * @param percentageBlue Reference to store the blue percentage.
     */
    void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) const;

    /**
     * @brief Compares the red and green color percentages.
     * 
     * @param percentageRed The red color percentage.
     * @param percentageGreen The green color percentage.
     * @return true If the first color is greater.
     * @return false Otherwise.
     */
    bool compareColors(double percentageRed, double percentageGreen) const;

    /**
     * @brief Checks if the new color value is close to the initial value within a tolerance.
     * 
     * @param initialValue The initial color value.
     * @param newValue The new color value.
     * @param tolerance The tolerance value.
     * @return true If the new value is close to the initial value.
     * @return false Otherwise.
     */
    bool isColorCloseTo(double initialValue, double newValue, double tolerance) const;

    /**
     * @brief Controls the motors based on the color comparison result.
     * 
     * @param isFirstGreater The result of the color comparison.
     */
    void controlMotors(bool isFirstGreater);

    /**
     * @brief Sets the motor speeds.
     * 
     * @param leftSpeed The speed of the left motor.
     * @param rightSpeed The speed of the right motor.
     */
    void setMotorSpeeds(uint16_t leftSpeed, uint16_t rightSpeed);

    /**
     * @brief Gets the state of whether the robot is on white.
     * 
     * @return true If the robot is on white.
     * @return false Otherwise.
     */
    bool getIsOnWhite() const;

    /**
     * @brief Sets the state of whether the robot is on white.
     * 
     * @param value The state to set.
     */
    void setIsOnWhite(bool value);

    /**
     * @brief Gets the calibrated red color value.
     * 
     * @return double The calibrated red color value.
     */
    double getCalibratedRed() const;

    /**
     * @brief Gets the calibrated green color value.
     * 
     * @return double The calibrated green color value.
     */
    double getCalibratedGreen() const;

private:
    ColorMode currentColorMode;
    LabyrinthConfig& config;
    double calibratedRed, calibratedGreen, calibratedBlue;
    bool isOnWhite;
};

#endif // LABYRINTH_MOVEMENT_H