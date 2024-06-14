#include  "ColorDetection.h"

void ColorDetection::begin(void){
    ColorDetection::configure(VEML_CONFIG{.mode = AUTO,.enabled = true,.exposureTime=MS40});
};
void ColorDetection::configure(VEML_CONFIG config){
     uint8_t configRegister = 0;
    switch(config.exposureTime)
    {
        case MS40:
            configRegister = 0x00;break;
        case MS80:
            configRegister = 0x01;break;
        case MS160:
            configRegister = 0x02;break;
        case MS320:
            configRegister = 0x03;break;
        case MS640:
            configRegister = 0x04;break;
        case MS1280:
            configRegister = 0x05;break;
    }
    configRegister = configRegister << 4;
    if(config.mode == MANUAL)
    {
        configRegister = configRegister | (0x01<<1);
    }
    if(!config.enabled)
    {
        configRegister = configRegister | 1;
    }
    ColorDetection::writeDoubleRegister(CMD_CONFIG,(uint16_t)configRegister);
};

uint16_t ColorDetection::getColorValue(color color){
   
    switch(color)
    {
        case VEML_RED:
            return readDoubleRegister(REG_RED); 
            break;
        case VEML_GREEN:
            return readDoubleRegister(REG_GREEN);
            break;
        case VEML_BLUE: 
            return readDoubleRegister(REG_BLUE);
            break;
        case VEML_WHITE:
            return readDoubleRegister(REG_WHITE);
            break;
        default:
            Serial.println("Color is not supported by the sensor");
            return 0;
    } 
};

uint16_t ColorDetection::readDoubleRegister(uint8_t regAddr){
    uint16_t result = 0;
    Wire.beginTransmission(VEML_ADDR);
    Wire.write(regAddr);
    if(Wire.endTransmission() != 0){
        Serial.printf("Reading Register %d failed",regAddr);
    }
    Wire.requestFrom(VEML_ADDR,2);
    uint8_t offset = 0;
    while(Wire.available()){
        result = result << 8;
        result = result | (Wire.read()<<offset);
        offset = offset + 8;
    }
    return result;
};

void ColorDetection::writeDoubleRegister(uint8_t regAddr, uint16_t data){
    //erst low dann high
    Wire.beginTransmission(VEML_ADDR);
    Wire.write(regAddr);
    Wire.write((uint8_t)(data&0x00FF));
    Wire.write((uint8_t)((data>>8)&0x00FF));
    if(Wire.endTransmission() != 0){
        Serial.printf("Reading Register %d failed",regAddr);
    }
};