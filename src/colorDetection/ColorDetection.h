/**
 * @file ColorDetecion.h
 * @author Hans Haupt
 * @brief Class that controls the colorsensor (VEML6040) of the dezibot.
 * @version 0.1
 * @date 2024-06-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ColorDetection_h
#define ColorDetection_h
#include <stdint.h>
#include <Wire.h>
#include <Arduino.h>
//Definitions for I2c
#define I2C_MASTER_SCL_IO           2      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           1      /*!< GPIO number used for I2C master data  */

//Chipadress of the VEML6040
#define VEML_ADDR                   0x10        /*!< Slave address of the MPU9250 sensor */

//CMDCodes for communicate with the VEML6040
#define CMD_CONFIG                  0x00
#define REG_RED                     0x08
#define REG_GREEN                   0x09
#define REG_BLUE                    0x0A
#define REG_WHITE                   0x0B


enum duration{
    MS40,
    MS80,
    MS160,
    MS320,
    MS640,
    MS1280
};

enum vemlMode{
    AUTO,
    MANUAL
};

struct VEML_CONFIG{
    vemlMode mode;
    bool enabled;
    duration exposureTime;
};


enum color{
    VEML_RED,
    VEML_GREEN,
    VEML_BLUE,
    VEML_WHITE
};
class ColorDetection{
public: 
    void begin(void);
    void configure(VEML_CONFIG config);
    uint16_t getColorValue(color color);
protected:
    uint16_t readDoubleRegister(uint8_t regAddr);
    void writeDoubleRegister(uint8_t regAddr, uint16_t data);
};
#endif //ColorDetection_h