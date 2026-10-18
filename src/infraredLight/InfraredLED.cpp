#include "InfraredLight.h"

#define pwmSpeedMode LEDC_LOW_SPEED_MODE

#define DUTY_CYCLE_MAX ((uint8_t)1023)

InfraredLED::InfraredLED(uint8_t pin, ledc_timer_t timer,
                         ledc_channel_t channel) {
  this->ledPin = pin;
  this->timer = timer;
  this->channel = channel;
};

void InfraredLED::begin(void) {
  // we want to change frequency instead of
  pwmTimer = ledc_timer_config_t{.speed_mode = pwmSpeedMode,
                                 .duty_resolution = LEDC_TIMER_10_BIT,
                                 .timer_num = this->timer,
                                 .freq_hz = 10000,
                                 .clk_cfg = LEDC_AUTO_CLK};
  ledc_timer_config(&pwmTimer);

  pwmChannel = ledc_channel_config_t{.gpio_num = this->ledPin,
                                     .speed_mode = pwmSpeedMode,
                                     .channel = this->channel,
                                     .intr_type = LEDC_INTR_DISABLE,
                                     .timer_sel = this->timer,
                                     .duty = 0,
                                     .hpoint = 0};
  ledc_channel_config(&pwmChannel);
};

void InfraredLED::turnOn(void) { InfraredLED::setState(true); };

void InfraredLED::turnOff(void) { InfraredLED::setState(false); };

void InfraredLED::setState(bool state) {
  ledc_set_freq(pwmSpeedMode, timer, 1);
  if (state) {
    ledc_set_duty(pwmSpeedMode, channel, DUTY_CYCLE_MAX);
  } else {
    ledc_set_duty(pwmSpeedMode, channel, 0);
  }
  ledc_update_duty(pwmSpeedMode, channel);
};

void InfraredLED::sendFrequency(uint16_t frequency) {
  ledc_set_freq(pwmSpeedMode, timer, frequency);
  ledc_set_duty(pwmSpeedMode, channel, 512);
  ledc_update_duty(pwmSpeedMode, channel);
};

void InfraredLED::setDutyCycle(uint16_t dutyCycle) {
  if (dutyCycle > DUTY_CYCLE_MAX) {
    dutyCycle = DUTY_CYCLE_MAX;
  }
  ledc_set_duty(pwmSpeedMode, channel, dutyCycle);
  ledc_update_duty(pwmSpeedMode, channel);
};
