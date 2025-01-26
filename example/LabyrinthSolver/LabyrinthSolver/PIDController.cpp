#include "PIDController.h"
#include <algorithm> 

PIDController::PIDController(double p, double i, double d)
    : kp(p), ki(i), kd(d), previousError(0), integral(0) {}

MotorStrength PIDController::calculateMotorStrength(int red, int green, int blue) {
    // Calculate the percentage of each color
    double total = red + green + blue;
    double redPercentage = (red / total) * 100.0;
    double greenPercentage = (green / total) * 100.0;

    // Calculate error: Target is equal contribution from red and green (50% each)
    double error = redPercentage - greenPercentage - 9.5;


    // Update integral and derivative
    integral += error;
    double derivative = error - previousError;

    // Compute correction using PID formula
    double correction = (kp * error) + (ki * integral) + (kd * derivative);

    // Update previous error
    previousError = error;

    // Calculate motor strengths
    int baseSpeed = 100; // Base speed for motors
    int leftMotor = static_cast<int>(baseSpeed - correction);
    int rightMotor = static_cast<int>(baseSpeed + correction);

    // Clamp motor strengths to valid range (0 to 100)
    leftMotor = std::max(0, std::min(100, leftMotor));
    rightMotor = std::max(0, std::min(100, rightMotor));

    return {leftMotor, rightMotor, error};
}
