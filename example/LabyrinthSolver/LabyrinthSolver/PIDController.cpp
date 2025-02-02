#include "PIDController.h"
#include <algorithm> 
#include <Dezibot.h>

extern Dezibot dezibot;

PIDController::PIDController(double p, double i, double d)
    : kp(p), ki(i), kd(d), previousError(0), integral(0) {}

MotorStrength PIDController::calculateMotorStrength(int red, int green, int blue) {
    // Calculate the percentage of each color
    double total = red + green + blue;
    double redPercentage = (red / total) * 100.0;
    double greenPercentage = (green / total) * 100.0;
    double bluePercentage = (blue / total) * 100.0;

    // Calculate error: Target is equal contribution from red and green (50% each)
    // WHite könnte faktor sein je mehr man sieht desto mehr wird der error faktor größer gemcaht
    // double error = redPercentage - greenPercentage - 5.5;
    // if (error < 0){
    //     error = error * 3;
    // }
    // error = error * error;

    double ratio = static_cast<double>(red) / (red + green);

    int G = 100;
    double error = (ratio - 0.53) * G;  // G is a gain so that error ~ ±1 at the extremes.


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

    Serial.print("R G B: ");
    Serial.print(redPercentage);
    Serial.print("  ");
    Serial.print(greenPercentage);
    Serial.print("  ");
    Serial.println(bluePercentage);
    
    Serial.print("Error: ");
    Serial.println(error);

    Serial.print("derivative: ");
    Serial.println(derivative);

    Serial.print("integral: ");
    Serial.println(integral);
    
    Serial.print("correction: ");
    Serial.println(correction);

    Serial.print("ratio: ");
    Serial.println(ratio);

    Serial.print("Lmotot Rmotor: ");
    Serial.print(leftMotor);
    Serial.print("  ");
    Serial.println(rightMotor);

    Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");

    return {leftMotor, rightMotor, error};
}
