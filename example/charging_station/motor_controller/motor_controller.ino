#include <AccelStepper.h>

#define RIGHT_STEP_PIN 14
#define RIGHT_DIR_PIN 12

#define LEFT_STEP_PIN 15
#define LEFT_DIR_PIN 2


const float stepsPerRevolution = 200;

int microstepSetting = 1;

AccelStepper stepperRight(AccelStepper::DRIVER, RIGHT_STEP_PIN, RIGHT_DIR_PIN);
AccelStepper stepperLeft(AccelStepper::DRIVER, LEFT_STEP_PIN, LEFT_DIR_PIN);

float convert_rotational_position_to_steps(float rotations) {
  return rotations * stepsPerRevolution * microstepSetting;
}
 
 int32_t position = 0;

 void setup() {
  Serial.begin(115200);

  Serial.println("+------------------+");
  Serial.println("| Motor Controller |");
  Serial.println("+------------------+");

  float MaxRPM = 100;
  float Max_Speed_StepsPerSec = microstepSetting * stepsPerRevolution * MaxRPM / 60;
  stepperRight.setMaxSpeed(Max_Speed_StepsPerSec);
  stepperLeft.setMaxSpeed(Max_Speed_StepsPerSec);

  
  Serial.printf("Set Max Speed = %f\n", Max_Speed_StepsPerSec);

  float AccelRPMperSec = 300;
  float Accel_StepsPerSec2 = microstepSetting * stepsPerRevolution * AccelRPMperSec / 60;
  stepperRight.setAcceleration(Accel_StepsPerSec2);
  stepperLeft.setAcceleration(Accel_StepsPerSec2);

  Serial.printf("Set Acceleration = %f\n", AccelRPMperSec);

  stepperRight.setCurrentPosition(position);
  stepperLeft.setCurrentPosition(position);

  
  Serial.printf("Set current position = %d\n", position);
  Serial.printf("Setup complete!\n");
}

void loop() {

  // put your main code here, to run repeatedly:
  position = 400;
  Serial.printf("Stepper.moveTo(%d)\n", position);
  stepperRight.moveTo(position);
  stepperLeft.moveTo(position);
  
  while (stepperRight.distanceToGo() + stepperLeft.distanceToGo() != 0) {
    if (stepperRight.distanceToGo() != 0) {
      stepperRight.run();
    }
    if (stepperLeft.distanceToGo() != 0) {
      stepperLeft.run();
    }
    
  }

  position = -400;  
  Serial.printf("Stepper.moveTo(%d)\n", position);
  stepperRight.moveTo(position);
  stepperLeft.moveTo(position);
  
  while (stepperRight.distanceToGo() + stepperLeft.distanceToGo() != 0) {
    if (stepperRight.distanceToGo() != 0) {
      stepperRight.run();
    }
    if (stepperLeft.distanceToGo() != 0) {
      stepperLeft.run();
    }
    
  }

  delay(500);
}
