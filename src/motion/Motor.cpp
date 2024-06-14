#include "Motion.h"

Motor::Motor(uint8_t pin, ledc_timer_t timer, ledc_channel_t channel){
    this->pin = pin;
    this->channel = channel;
    this->timer = timer;
    this->duty = 0;
};

void Motor::begin(void){
    pinMode(this->pin,OUTPUT);
    ledc_channel_config_t channelConfig = {
        .gpio_num       = this->pin,
        .speed_mode     = LEDC_MODE,
        .channel        = this->channel,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = this->timer,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&channelConfig);
    Serial.println("Motor begin done");
};

void Motor::setSpeed(uint16_t duty){
    
    int difference = duty-this->getSpeed();
    if (difference > 0){
        for(int i = 0;i<difference;i+=difference/20){
            this->duty += difference/20;
            ledc_set_duty(LEDC_MODE,this->channel,duty);
            ledc_update_duty(LEDC_MODE,this->channel);
            delayMicroseconds(5);
        }
    } else {
        for(int i = 0;i>difference;i-=abs(difference/20)){
            this->duty -= abs(difference/20);
            ledc_set_duty(LEDC_MODE,this->channel,duty);
            ledc_update_duty(LEDC_MODE,this->channel);
            delayMicroseconds(5);
        }
    }

};

uint16_t Motor::getSpeed(void){
    return this->duty;
};
