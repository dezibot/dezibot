/**
 * @file LabyrinthConfig.h
 * @brief Header file for the LabyrinthConfig class.
 * @version 0.1
 * @date 2023-10-10
 * 
 * @author Vladislav Antonov
 * 
 * This file contains the declaration of the LabyrinthConfig class, which holds configuration settings for the labyrinth solver.
 */

#ifndef LABYRINTH_CONFIG_H
#define LABYRINTH_CONFIG_H

#include <cstdint>

class LabyrinthConfig {
public:
    /**
     * @brief Gets the base speed of the robot.
     * @return uint16_t The base speed.
     */
    uint16_t getBaseSpeed() const;

    /**
     * @brief Gets the maximum speed of the robot.
     * @return uint16_t The maximum speed.
     */
    uint16_t getMaxSpeed() const;

    /**
     * @brief Gets the rotation speed of the robot.
     * @return uint16_t The rotation speed.
     */
    uint16_t getRotateSpeed() const;

    /**
     * @brief Gets the red color scale factor.
     * @return double The red color scale factor.
     */
    double getRedScale() const;

    /**
     * @brief Gets the green color scale factor.
     * @return double The green color scale factor.
     */
    double getGreenScale() const;

    /**
     * @brief Gets the rotation duration.
     * @return int The rotation duration in milliseconds.
     */
    int getRotateDuration() const;

    /**
     * @brief Gets the duration for moving straight.
     * @return int The duration for moving straight in milliseconds.
     */
    int getMoveStraightDuration() const;

    /**
     * @brief Gets the tolerance for detecting white color.
     * @return double The white color tolerance.
     */
    double getWhiteTolerance() const;

    /**
     * @brief Sets the base speed of the robot.
     * @param speed The base speed.
     */
    void setBaseSpeed(uint16_t speed);

    /**
     * @brief Sets the maximum speed of the robot.
     * @param speed The maximum speed.
     */
    void setMaxSpeed(uint16_t speed);

    /**
     * @brief Sets the rotation speed of the robot.
     * @param speed The rotation speed.
     */
    void setRotateSpeed(uint16_t speed);

    /**
     * @brief Sets the red color scale factor.
     * @param scale The red color scale factor.
     */
    void setRedScale(double scale);

    /**
     * @brief Sets the green color scale factor.
     * @param scale The green color scale factor.
     */
    void setGreenScale(double scale);

    /**
     * @brief Sets the rotation duration.
     * @param duration The rotation duration in milliseconds.
     */
    void setRotateDuration(int duration);

    /**
     * @brief Sets the duration for moving straight.
     * @param duration The duration for moving straight in milliseconds.
     */
    void setMoveStraightDuration(int duration);

    /**
     * @brief Sets the tolerance for detecting white color.
     * @param tolerance The white color tolerance.
     */
    void setWhiteTolerance(double tolerance);

private:
    uint16_t baseSpeed = 3900;
    uint16_t maxSpeed = 8191;
    uint16_t rotateSpeed = 2500;
    double redScale = 1.00;
    double greenScale = 0.98;
    int rotateDuration = 2000;
    int moveStraightDuration = 1500;
    double whiteTolerance = 0.7;
};

#endif // LABYRINTH_CONFIG_H