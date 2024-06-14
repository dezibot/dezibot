/**
 * @file Motion.cpp
 * @author Jonathan Schulze, Nick Hübenthal, Hans Haupt
 * @brief Implementation of the Motion class.
 * @version 0.2
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Motion.h"


// Initialize the movement component.


void Motion::begin(void) {
    ledc_timer_config_t motor_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = DUTY_RES,
        .timer_num        = TIMER,
        .freq_hz          = FREQUENCY,  
        .clk_cfg          = LEDC_AUTO_CLK
    }; 
    ledc_timer_config(&motor_timer);
    Motion::left.begin();
    Motion::right.begin();
    detection.begin();
};
void Motion::moveTask(void * args) {
    uint32_t runtime = (uint32_t)args;

    Motion::left.setSpeed(LEFT_MOTOR_DUTY);
    Motion::right.setSpeed(RIGHT_MOTOR_DUTY);
    Motion::xLastWakeTime = xTaskGetTickCount();
    while(1){
        if(runtime>40||runtime==0){
            vTaskDelayUntil(&xLastWakeTime,40);
            runtime -= 40;
            //calc new parameters
            //set new parameters
            int fifocount = detection.getDataFromFIFO(buffer);
            int rightCounter = 0;
            int leftCounter = 0;
            int changerate = 0;
            for(int i = 0;i<fifocount;i++){
                if(buffer[i].gyro.z>correctionThreshold){
                    rightCounter++;
                } else if(buffer[i].gyro.z<-correctionThreshold){
                    leftCounter++;
                }
            }
            int difference = abs(leftCounter-rightCounter);
            if (difference>25){
                changerate = 200;
            } else if(difference>20){
                changerate = 100;
            } else if(difference >15){
                changerate = 50;
            } else if(difference > 10){
                changerate = 20;
            } else{
                changerate = 5; 
            }

            if(leftCounter>rightCounter){ //rotates anticlock
                LEFT_MOTOR_DUTY+=changerate;
                RIGHT_MOTOR_DUTY-=changerate;
            } else if(leftCounter<rightCounter){
                LEFT_MOTOR_DUTY-=changerate;
                RIGHT_MOTOR_DUTY+=changerate;
            } 

            Motion::left.setSpeed(LEFT_MOTOR_DUTY);
            Motion::right.setSpeed(RIGHT_MOTOR_DUTY);
        } else {
            vTaskDelayUntil(&xLastWakeTime,runtime);
            Motion::left.setSpeed(0);
            Motion::right.setSpeed(0);
            vTaskDelete(xMoveTaskHandle);
        }
    }
};

// Move forward for a certain amount of time.
void Motion::move(uint32_t moveForMs, uint baseValue) {    
       if(xMoveTaskHandle){
            vTaskDelete(xMoveTaskHandle);
            xMoveTaskHandle = NULL;
       }
       if(xClockwiseTaskHandle){

            vTaskDelete(xClockwiseTaskHandle);
            xClockwiseTaskHandle = NULL;
       }
       if(xAntiClockwiseTaskHandle){
            vTaskDelete(xAntiClockwiseTaskHandle);
            xAntiClockwiseTaskHandle = NULL;

       }
       LEFT_MOTOR_DUTY = baseValue;
       RIGHT_MOTOR_DUTY = baseValue;
        xTaskCreate(moveTask, "Move", 4096, (void*)moveForMs, 10, &xMoveTaskHandle);
        
};

void Motion::leftMotorTask(void * args) {
     uint32_t runtime = (uint32_t)args;
     if(xMoveTaskHandle){
        vTaskDelete(xMoveTaskHandle);
        xMoveTaskHandle = NULL;
    }
    if(xAntiClockwiseTaskHandle){
        vTaskDelete(xAntiClockwiseTaskHandle);
        xAntiClockwiseTaskHandle = NULL;
    }
    Motion::right.setSpeed(0);
    Motion::left.setSpeed(LEFT_MOTOR_DUTY);
    while(1){
        if((runtime>40)||(runtime==0)){
            vTaskDelayUntil(&xLastWakeTime,40);
            runtime -=40;
        } else {
            vTaskDelayUntil(&xLastWakeTime,runtime);
            Motion::left.setSpeed(0);
            vTaskDelete(xClockwiseTaskHandle);
        }
        vTaskDelayUntil(&xLastWakeTime,40);
    }
};

// Rotate clockwise for a certain amount of time.
void Motion::rotateClockwise(uint32_t rotateForMs,uint baseValue) {
    LEFT_MOTOR_DUTY = baseValue;
    RIGHT_MOTOR_DUTY = baseValue;
    if (rotateForMs > 0){
        if(xClockwiseTaskHandle){
            vTaskDelete(xClockwiseTaskHandle);
        }
        xTaskCreate(leftMotorTask, "LeftMotor", 4096, (void*)rotateForMs, 10, &xClockwiseTaskHandle);
    } else {
        Motion::left.setSpeed(LEFT_MOTOR_DUTY);
        Motion::right.setSpeed(0);
    }
};

void Motion::rightMotorTask(void * args) {
    uint32_t runtime = (uint32_t)args;
     if(xMoveTaskHandle){
        vTaskDelete(xMoveTaskHandle);
        xMoveTaskHandle = NULL;
    }
    if(xClockwiseTaskHandle){
        vTaskDelete(xClockwiseTaskHandle);
        xClockwiseTaskHandle = NULL;
    }
    Motion::right.setSpeed(RIGHT_MOTOR_DUTY);
    Motion::left.setSpeed(0);
    while(1){
        if(runtime>40||runtime==0){
            vTaskDelayUntil(&xLastWakeTime,40);
            runtime -= 40;
        } else {
            vTaskDelayUntil(&xLastWakeTime,runtime);
            Motion::right.setSpeed(0);
            vTaskDelete(xAntiClockwiseTaskHandle);
        }
    }
};

// Rotate anticlockwise for a certain amount of time.
void Motion::rotateAntiClockwise(uint32_t rotateForMs,uint baseValue) {
    LEFT_MOTOR_DUTY = baseValue;
    RIGHT_MOTOR_DUTY = baseValue;
    if(rotateForMs > 0){
        if(xAntiClockwiseTaskHandle){
            vTaskDelete(xAntiClockwiseTaskHandle);
        }
        xTaskCreate(rightMotorTask, "RightMotor", 4096, (void*)rotateForMs, 10, &xAntiClockwiseTaskHandle);
    } else {
        Motion::right.setSpeed(RIGHT_MOTOR_DUTY);
        Motion::left.setSpeed(0);
    }
};

void Motion::stop(void){
    if(xMoveTaskHandle){
        vTaskDelete(xMoveTaskHandle);
        xMoveTaskHandle = NULL;
    }
    if(xAntiClockwiseTaskHandle){
        vTaskDelete(xAntiClockwiseTaskHandle);
        xAntiClockwiseTaskHandle = NULL;
    }
    if(xClockwiseTaskHandle){
        vTaskDelete(xClockwiseTaskHandle);
        xClockwiseTaskHandle = NULL;
    }
    Motion::left.setSpeed(0);
    Motion::right.setSpeed(0);
}
 
