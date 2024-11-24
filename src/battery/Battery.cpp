/**
 * @file Battery.cpp
 * @author Florian Schmidt
 * @brief Implementation of the Battery class
 * @version 0.1
 * @date 2024-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Battery.h"
#include <Arduino.h>

void Battery::begin(void){
    pinMode(BAT_ADC_EN_PIN, OUTPUT);
    pinMode(VUSB_SENS_PIN, INPUT_PULLUP);
};

bool Battery::isCharging(void){
    bool vusb_inv = (bool) digitalRead(VUSB_SENS_PIN);
    return !vusb_inv;
};

float Battery::getVoltage(void){
    uint16_t adcValue = 0;
    const uint8_t samples = 8; // ensure samples*4095 <= UINT16_MAX

    digitalWrite(BAT_ADC_EN_PIN, HIGH);
    for (int i = 0; i < samples; i++)
    {
        adcValue += analogRead(BAT_ADC_PIN);
        ets_delay_us(125);
    }
    digitalWrite(BAT_ADC_EN_PIN, LOW);
    
    adcValue /= samples;

    const float n = 57.5f;
    const float fact = 3.4054f;
    float vBat = ((static_cast<float>(adcValue) + n) / 4095) * fact * 3.7f;
    return vBat;
};

uint8_t Battery::getBatteryLevel(void){
    return voltsToBatLevel(getVoltage(), isCharging());
};

BatteryInfo Battery::getAll(void){
    float vBat = getVoltage();
    bool charging = isCharging();
    uint8_t batteryLevel = voltsToBatLevel(vBat, charging);

    BatteryInfo info;
    info.charging = charging;
    info.voltage = vBat;
    info.batteryLevel = batteryLevel;
    return info;
}

uint8_t Battery::voltsToBatLevel(float vBat, bool isCharging){
    const float *coefficients;

    switch (isCharging)
    {
    case true:
        if (vBat < 3.57)
            return 1;
        if (vBat > 4.22)
            return 100;
        coefficients = CHARGE_COEFFS;
        break;
    case false:
        if (vBat < 3.36)
            return 1;
        if (vBat > 4.13)
            return 100;
        coefficients = DISCHARGE_COEFFS;
        break;
    }
    
    // 5th order polynomial, this can definitely be optimized
    // takes around 140 us to calculate
    float level = coefficients[0] * pow(vBat, 5)+ coefficients[1] * pow(vBat, 4)
    + coefficients[2] * pow(vBat, 3) + coefficients[3] * pow(vBat, 2)
    + coefficients[4] * vBat + coefficients[5];

    return static_cast<uint8_t>(round(level));
};
