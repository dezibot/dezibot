/**
 * @file ColorDetecion.h
 * @author Nico Schramm, Ines Rohrbach, Hans Haupt
 * @brief Class that controls the color sensor (VEML6040) of the dezibot.
 * 
 * This module uses the VEML6040 library (version 0.3.2) by thewknd (MIT license).
 * 
 * @version 0.2
 * @date 2024-11-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ColorDetection_h
#define ColorDetection_h

#include <stdint.h>
#include <Wire.h>
#include <Arduino.h>
#include <veml6040.h>

enum duration {
    MS40,
    MS80,
    MS160,
    MS320,
    MS640,
    MS1280
};

enum vemlMode {
    AUTO,
    MANUAL
};

struct VEML_CONFIG {
    // force mode
    vemlMode mode;
    
    // chip shutdown setting
    bool enabled;
    
    // integration time
    duration exposureTime;
};

enum color {
    VEML_RED,
    VEML_GREEN,
    VEML_BLUE,
    VEML_WHITE
};

class ColorDetection {
public:
    /**
     * @brief Start RBGW sensor with default configuration.
     * 
     */
    void beginAutoMode();
    
    /**
     * @brief Begin RGBW sensor with passed configuration values.
     * 
     * @param config configuration for VEML6040 sensor
     */
    void configure(VEML_CONFIG config);

    /**
     * @brief Get color value of RGBW sensor.
     * 
     * @param color RGBW color which to get
     * @return uint16_t color value
     */
    uint16_t getColorValue(color color);

    /**
     * @brief Get the ambient light in lux.
     * 
     * @return float ambient light in lux.
     */
    float getAmbientLight();

protected:
    VEML6040 rgbwSensor;
};
#endif //ColorDetection_h