/**
 * @file Motion.h
 * @author Jonathan Schulze, Nick Hübenthal, Hans Haupt
 * @brief This component controls the ability to rotate and change position. 
 * @version 0.2
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef Motion_h
#define Motion_h
#include <stdint.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/ledc.h"
#include "motionDetection/MotionDetection.h"
#define LEDC_MODE          LEDC_LOW_SPEED_MODE
#define TIMER              LEDC_TIMER_2
#define CHANNEL_LEFT       LEDC_CHANNEL_3 
#define CHANNEL_RIGHT      LEDC_CHANNEL_4  
#define DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz
#define DEFAULT_BASE_VALUE  3900
class Motor{
    public:
        Motor(uint8_t pin, ledc_timer_t timer, ledc_channel_t channel);
        
        /**
         * @brief Initializes the motor 
         */
        void begin(void);
        
        /**
         * @brief Set the Speed by changing the pwm. To avoid current peaks, a linear ramp-up is used.
         * 
         * @attention it is requried at any time to use that method to access the motors or methods of the motionclass to avoid such peaks.
         * 
         * @param duty the duty cyle that should be set, can be between 0-8192
         */
        void setSpeed(uint16_t duty);
        
        /**
         * @brief returns the currently activ speed
         * 
         * @return current speedvalue of the motor
         */
        uint16_t getSpeed(void); 
    protected:
        uint8_t pin;
        ledc_timer_t timer;
        ledc_channel_t channel;
       
        uint16_t duty;
};

class Motion{
protected:
    static inline uint16_t RIGHT_MOTOR_DUTY = DEFAULT_BASE_VALUE;
    static inline uint16_t LEFT_MOTOR_DUTY = DEFAULT_BASE_VALUE;
    static const int MOTOR_RIGHT_PIN = 11; 
    static const int MOTOR_LEFT_PIN = 12;
    static void moveTask(void * args);
    static void leftMotorTask(void * args);
    static void rightMotorTask(void * args);
    static inline TaskHandle_t xMoveTaskHandle = NULL;
    static inline TaskHandle_t xClockwiseTaskHandle = NULL;
    static inline TaskHandle_t xAntiClockwiseTaskHandle = NULL;
    static inline TickType_t xLastWakeTime;

    static inline FIFO_Package* buffer = new FIFO_Package[64];
    static inline int correctionThreshold = 150; 

public:
    //Instances of the motors, so they can also be used from outside to set values for the motors directly.
    static inline Motor left = Motor(MOTOR_LEFT_PIN,TIMER,CHANNEL_LEFT);
    static inline Motor right = Motor(MOTOR_RIGHT_PIN,TIMER,CHANNEL_RIGHT);
    
    //MotionDetection instance, for motion Correction and user (access with dezibot.motion.detection)
    static inline MotionDetection detection;

    /**
     * @brief Initialize the movement component.
     * 
    */
    void begin(void);

    /**
     * @brief Move forward for a certain amount of time.
     * Call with moveForMs 0 will start movement, that must be stopped explicit by call to stop().
     * The function applys a basic algorithm to improve the straigthness of the movement. 
     * Lifting the robot from the desk may corrupt the results and is not recommended.
     *  
     * @param moveForMs Representing the duration of forward moving in milliseconds.
     * @param baseValue The value that is used to start with the calibrated movement. Defaults to 3900. 
     * If the Dezibot is not moving forward at all increasing the value may help. If the robot is just jumping up and down but not forward, try a lower value. 
    */
    static void move(uint32_t moveForMs=0,uint baseValue=DEFAULT_BASE_VALUE);

    /**
     * @brief Rotate clockwise for a certain amount of time.
     * Call with moveForMs 0 will start movement, that must be stopped explicit by call to stop().
     * @param rotateForMs Representing the duration of rotating clockwise in milliseconds, or 0 to rotate until another movecmd is issued. Default is 0
     * @param baseValue The value that is used to start with the calibrated movement (not released yet, currently just the used value)
    */
    static void rotateClockwise(uint32_t rotateForMs=0,uint baseValue=DEFAULT_BASE_VALUE);
    
    /**
     * @brief Rotate anticlockwise for a certain amount of time.
     * Call with moveForMs 0 will start movement, that must be stopped explicit by call to stop().
     * @param rotateForMs Representing the duration of rotating anticlockwise in milliseconds or 0 to let the robot turn until another movecommand is issued. Default is 0.
     * @param baseValue The value that is used to start with the calibrated movement (not released yet, currently just the used value).
    */
    static void rotateAntiClockwise(uint32_t rotateForMs=0,uint baseValue=DEFAULT_BASE_VALUE);

    /**
     * @brief stops any current movement, no matter if timebased or endless
     * 
     */
    static void stop(void);

    /**
     * @brief Does the same as the move function, but this function does not apply any kind of algorithm to improve the result.
     * 
     * @param moveForMs how many ms should the robot move, or 0 to let the robot move until another move command is mentioned, default is 0
     * @param baseValue the duty value that is used for the movement, default is 0
     */
    static void moveWithoutCorrection(uint32_t moveForMs=0, uint baseValue = DEFAULT_BASE_VALUE);

};


#endif //Motion_h