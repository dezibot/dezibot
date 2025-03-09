#include  "ColorDetection.h"

#include <logger/Logger.h>

void ColorDetection::beginAutoMode(void) {
    const VEML_CONFIG DEFAULT_CONFIG = VEML_CONFIG {
        .mode = AUTO,
        .enabled = true,
        .exposureTime = MS320 };
    ColorDetection::configure(DEFAULT_CONFIG);

    Logger::getInstance().logDebug("Successfully started ColorDetection module");
};

void ColorDetection::configure(VEML_CONFIG config) {
    rgbwSensor.begin();

    uint8_t configuration = 0;

    switch(config.exposureTime) {
        case MS40:
            configuration += VEML6040_IT_40MS;
            break;
        case MS80:
            configuration += VEML6040_IT_80MS;
            break;
        case MS160:
            configuration += VEML6040_IT_160MS;
            break;
        case MS320:
            configuration += VEML6040_IT_320MS;
            break;
        case MS640:
            configuration += VEML6040_IT_640MS;
            break;
        case MS1280:
            configuration += VEML6040_IT_1280MS;
            break;
    }

    configuration += (config.mode == MANUAL) ? VEML6040_AF_FORCE : VEML6040_AF_AUTO;
    configuration += config.enabled ? VEML6040_SD_ENABLE : VEML6040_SD_DISABLE;
    
    rgbwSensor.setConfiguration(configuration);
};

uint16_t ColorDetection::getColorValue(color color){
    uint16_t value;

    switch(color) {
        case VEML_RED:
            value = rgbwSensor.getRed();
            break;
        case VEML_GREEN:
            value = rgbwSensor.getGreen();
            break;
        case VEML_BLUE: 
            value = rgbwSensor.getBlue();
            break;
        case VEML_WHITE:
            value = rgbwSensor.getWhite();
            break;
        default:
            Serial.println("Color is not supported by the sensor");
            value =  0;
            break;
    }

    Logger::getInstance().logInfo(
        "Getting color value for color Sensor "
        + std::to_string(color)
        + " with value: "
        + std::to_string(value)
    );

    return value;
};

float ColorDetection::getAmbientLight() {
    float value = rgbwSensor.getAmbientLight();

    Logger::getInstance().logInfo(
        "Getting ambient light with value: "
        + std::to_string(value)
    );

    return value;
};
