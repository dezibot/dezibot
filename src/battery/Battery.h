 /**
 * @file Battery.h
 * @author Florian Schmidt
 * @brief Class for retrieving battery information, including voltage, approximate charge percentage,
 * and power connection status for the Dezibot.
 * @version 0.1
 * @date 2024-11-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef Battery_h
#define Battery_h

#include <stdint.h>

struct BatteryInfo {
    bool charging;
    float voltage;
    uint8_t batteryLevel;
};

class Battery {
public:
    /**
     * @brief Initialize gpio pins for battery monitoring
     */
    void begin();

    /**
     * @brief Check whether USB voltage is present
     * (returns true even if charging is complete).
     * 
     * If the battery voltage is also required, use getAll() instead.
     * 
     * @return bool charger attached
     */
    bool isCharging();
    
    /**
     * @brief Read the current voltage of the battery
     * 
     * If the battery voltage is also required, use getAll() instead.
     * 
     * @return float vBat (~3.2-4.2) 
     */
    float getVoltage();

    /**
     * @brief Approximate the current state of charge, compensating for charging/discharging voltages
     * 
     * If charging status/voltage is also required, use getAll() instead
     * 
     * @return uint8_t batteryLevel (1-100)
     */
    uint8_t getBatteryLevel();

    /**
     * @brief get all battery information at once (charging status, voltage, battery level).
     * Should only be used when the battery level plus any other information is needed,
     * as battery level approximation is expensive (but needs charging status and voltage regardless)
     * 
     * @return BatteryInfo {bool charging, float voltage, uint8_t batteryLevel}
     */
    BatteryInfo getAll();

protected:
    uint8_t voltsToBatLevel(float vBat, bool isCharging);
    static inline uint8_t BAT_ADC_EN_PIN = 9;
    static inline uint8_t BAT_ADC_PIN = 10;
    static inline uint8_t VUSB_SENS_PIN = 38;
    // when changing the declared lengths, their definitions and Battery::voltsToBatLevel() must be updated
    static const float CHARGE_COEFFS[6];
    static const float DISCHARGE_COEFFS[6];
};

#endif //Battery_h
