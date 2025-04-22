#include  "ColorDetection.h"

void ColorDetection::beginAutoMode(void) {
    const VEML_CONFIG DEFAULT_CONFIG = VEML_CONFIG {
        .mode = AUTO,
        .enabled = true,
        .exposureTime = MS320 };
    ColorDetection::configure(DEFAULT_CONFIG);
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
    switch(color) {
        case VEML_RED:
            return rgbwSensor.getRed();
        case VEML_GREEN:
            return rgbwSensor.getGreen();
        case VEML_BLUE: 
            return rgbwSensor.getBlue();
        case VEML_WHITE:
            return rgbwSensor.getWhite();
        default:
            Serial.println("Color is not supported by the sensor");
            return 0;
    } 
};

float ColorDetection::getAmbientLight() {
    return rgbwSensor.getAmbientLight();
};
