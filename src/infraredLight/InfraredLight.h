/**
 * @file InfraredLight.h
 * @author Hans Haupt (hans.haupt@dezibot.de)
 * @brief Provides basic controls for the infrared LEDs of the robot.
 * @version 0.1
 * @date 2024-04-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef InfraredLight_h
#define InfraredLight_h
#include <stdint.h>
#include <Arduino.h>
#include "driver/ledc.h"


class InfraredLED{
    public: 
        InfraredLED(uint8_t pin, ledc_timer_t timer, ledc_channel_t channel);
        void begin(void);
        /**
         * @brief enables selected LED
         * 
         */
        void turnOn(void);
        /**
         * @brief disables selected LED
         * 
         * @param led 
         */
        void turnOff(void);
        /**
         * @brief changes state of selected LED depending on the state
         * 
         * @param led which led will be affected
         * @param state true if led should be turned on, else false
         */
        void setState(bool state);
        /**
         * @brief starts flashing the IRLed with a specific frequency 
         * Won't stop automatically, must be stopped by calling any other IR-Method
         * 
         * @param frequency 
         */
        void sendFrequency(uint16_t frequency);
    protected:
        uint8_t ledPin;
        ledc_timer_t timer;
        ledc_channel_t channel;
        ledc_timer_config_t pwmTimer;
        ledc_channel_config_t pwmChannel;
};

class InfraredLight{
public:
    //Do something for correct resource sharing
    InfraredLED bottom = InfraredLED(IRBottomPin,LEDC_TIMER_0,LEDC_CHANNEL_0);
    InfraredLED front  = InfraredLED(IRFrontPin,LEDC_TIMER_1,LEDC_CHANNEL_1);
    void begin(void);

protected:
    static const uint8_t IRFrontPin = 14;
    static const uint8_t IRBottomPin = 13;
};


#endif //InfraredLight_h