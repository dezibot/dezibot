#include "PIDController.h"
#include <algorithm> 
#include <Dezibot.h>
#include <cmath> 

extern Dezibot dezibot;

PIDController::PIDController(double p, double i, double d)
    : kp(p), ki(i), kd(d), previousError(0), integral(0) {}

MotorStrength PIDController::calculateMotorStrength(int red, int green, int blue, ColorMode colorMode) {
    // Calculate the percentage of each color
    double total = red + green + blue;
    double redPercentage = (red / total) * 100.0;
    double greenPercentage = (green / total) * 100.0;
    double bluePercentage = (blue / total) * 100.0;


    if (bluePercentage < 27){
        dezibot.display.print("blauProzent: ");
        dezibot.display.println(bluePercentage);
        if(colorMode == RED_LEFT){
            return {55, 65, 0};
        }else {
            return {65, 55, 0};
        }
    }

    if (greenPercentage > 35){
        dezibot.display.print("greenPercen: ");
        dezibot.display.println(greenPercentage);

        if(colorMode == RED_LEFT){
            return {80, 70, 0};
        }else {
            return {70, 80, 0};
        }
    }
    double ratio = static_cast<double>(red) / (red + green);

    int G = 100;
    double error = (ratio - 0.53) * (-G);  // G is a gain so that error ~ ±1 at the extremes.


    // Update integral and derivative
    integral += error;
    double derivative = error - previousError;

    // Compute correction using PID formula
    double correction = (kp * error) + (ki * integral) + (kd * derivative);

    // Update previous error
    previousError = error;

    // Calculate motor strengths
    int baseSpeed = 100; // Base speed in % for motors


    int leftMotor = 0;
    int rightMotor = 0;

    if (colorMode == RED_LEFT){
        leftMotor = static_cast<int>(baseSpeed - correction);
        rightMotor = static_cast<int>(baseSpeed + correction);
    } else {
        leftMotor = static_cast<int>(baseSpeed + correction);
        rightMotor = static_cast<int>(baseSpeed - correction);
    }

    leftMotor = std::max(0, std::min(baseSpeed, leftMotor));
    rightMotor = std::max(0, std::min(baseSpeed, rightMotor));


    // Serial.print("R G B: ");
    // Serial.print(redPercentage);
    // Serial.print("  ");
    // Serial.print(greenPercentage);
    // Serial.print("  ");
    // Serial.println(bluePercentage);
    
    // Serial.print("Error: ");
    // Serial.println(error);

    // Serial.print("derivative: ");
    // Serial.println(derivative);

    // Serial.print("integral: ");
    // Serial.println(integral);
    
    // Serial.print("correction: ");
    // Serial.println(correction);

    // Serial.print("ratio: ");
    // Serial.println(ratio);

    // Serial.print("Lmotot Rmotor: ");
    // Serial.print(leftMotor);
    // Serial.print("  ");
    // Serial.println(rightMotor);

    // Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");

    return {leftMotor, rightMotor, error};
}

void PIDController::reset() {
    previousError = 0;
    integral = 0;
}
