/**
 * @file PIDController.h
 * @author Vladislav Antonov, Max Mager
 * @brief PID Controller to control the motor speede when moving an a line.
 * 
 * @version 1.0.0
 * @date 2025-02-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H


#include "CommonDefinitions.h"


class PIDController {
private:
    double kp, ki, kd; // PID constants
    double previousError;
    double integral;

public:
    /**
     * @brief Constructs a PIDController with the specified PID coefficients.  
     * Initializes the proportional, integral, and derivative gains,  
     * as well as the previous error and integral term.  
     * @param p The proportional gain (Kp).  
     * @param i The integral gain (Ki).  
     * @param d The derivative gain (Kd).  
     */
    PIDController(double p, double i, double d);

    /**
     * @brief Calculates motor strength using a PID controller based on color sensor data.  
     * Determines color percentages, applies predefined thresholds for blue and green,  
     * and computes an error ratio for fine-tuned motor control.  
     * Uses a PID algorithm to adjust motor speed corrections dynamically.  
     * @param red The detected red color value.  
     * @param green The detected green color value.  
     * @param blue The detected blue color value.  
     * @param colorMode The color mode indicating which side prioritizes red detection.  
     * @return A MotorStrength structure containing the calculated left and right motor speeds, along with the error value.  
     */
    MotorStrength calculateMotorStrength(int red, int green, int blue, ColorMode colorMode);
    /**
     * @brief Resets the PID controller's internal state.  
     * Sets the previous error and integral term to zero.  
     */
    void reset();
};

#endif // PIDCONTROLLER_H
