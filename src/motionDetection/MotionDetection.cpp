#include "MotionDetection.h"
#include <math.h>

MotionDetection::MotionDetection(){
    handler = new SPIClass(FSPI);
};

void MotionDetection::begin(void){
    pinMode(34,OUTPUT);
    digitalWrite(34,HIGH);
    handler->begin(36,37,35,34);
    // set Accel and Gyroscop to Low Noise
    this->writeRegister(PWR_MGMT0,0x1F);
    //busy Wait for startup 
    delayMicroseconds(250);
    //set accelconfig
    this->writeRegister(0x21,0x05);
    //set Gyroconfig
    this->writeRegister(0x20,0x25);
    //set Gyro Filter
    this->writeRegister(0x23,0x37);
    //Enable Gyro and Acceldata in FIFO
    this->initFIFO();
};
void MotionDetection::end(void){
    this->writeRegister(PWR_MGMT0,0x00);
};
IMUResult MotionDetection::getAcceleration(){
    IMUResult result;
    result.x = readRegister(ACCEL_DATA_X_HIGH)<<8 | readRegister(ACCEL_DATA_X_LOW);
    result.y = readRegister(ACCEL_DATA_Y_HIGH)<<8 | readRegister(ACCEL_DATA_Y_LOW);
    result.z = readRegister(ACCEL_DATA_Z_HIGH)<<8 | readRegister(ACCEL_DATA_Z_LOW);
    return result;
};
IMUResult MotionDetection::getRotation(){
    IMUResult result;
    result.x = readRegister(GYRO_DATA_X_HIGH) <<8;
    result.x |= readRegister(GYRO_DATA_X_LOW);
    result.y = readRegister(GYRO_DATA_Y_HIGH)<<8;
    result.y |= readRegister(GYRO_DATA_Y_LOW);
    result.z = readRegister(GYRO_DATA_Z_HIGH)<<8;
    result.z |= readRegister(GYRO_DATA_Z_LOW);
    return result;
};
float MotionDetection::getTemperature(){
    int16_t raw_temperatur = readRegister(REG_TEMP_HIGH)<<8;
    raw_temperatur |= readRegister(REG_TEMP_LOW); 
    return raw_temperatur/128 +25;
};

int8_t MotionDetection::getWhoAmI(){
    return readRegister(WHO_AM_I);    
};

bool MotionDetection::isShaken(uint32_t threshold ,uint8_t axis){
    IMUResult measurment1;
    IMUResult measurment2;
    uint count = 0;
    for(uint i = 0;i<20;i++){
        measurment1 = this->getAcceleration();
        delayMicroseconds(10);
        measurment2 = this->getAcceleration();
        if(
            (((axis && xAxis) > 0) && (abs(abs(measurment1.x)-abs(measurment2.x))>threshold)) ||
            (((axis && yAxis) > 0) && (abs(abs(measurment1.y)-abs(measurment2.y))>threshold)) ||
            (((axis && zAxis) > 0) && (abs(abs(measurment1.z)-abs(measurment2.z))>threshold))){
                count++;
        }
        delayMicroseconds(15);
    }
    return (count > 6);
};

void MotionDetection::calibrateZAxis(uint gforceValue){
    this->gForceCalib = gforceValue;
};

Orientation MotionDetection::getTilt(){
    uint tolerance = 200;
    IMUResult reading = this->getAcceleration();
    bool flipped = reading.z < 0;
    float accelrationStrenght = sqrt(reading.x*reading.x+reading.y*reading.y+reading.z*reading.z);
    //check if reading is valid
    if (abs(accelrationStrenght-this->gForceCalib) > tolerance ){
        //total accelration is not gravitational force, error
        return Orientation{INT_MAX,INT_MAX};
    }

    //calculates the angle between the two axis, therefore value between 0-90
    int yAngle;
    int xAngle;
    if(reading.z != 0){
        yAngle = atan(float(reading.x)/reading.z)*180/3.1415+0.5;
        xAngle = atan(float(reading.y)/reading.z)*180/3.1415+0.5;
    } else {
        yAngle = 90*(reading.x > 0) - (reading.x < 0);
        xAngle = 90*(reading.y > 0) - (reading.y < 0);
    }

    //shift quadrants depending on signum
        //esp is facing down (normal position)
    if (reading.z > 0){
        if(reading.y < 0){
            xAngle = xAngle+180;
        } else{
            xAngle = -1*(180-xAngle);
        }

        if(reading.x < 0){
            yAngle = yAngle+180;
        } else{
            yAngle = -1*(180-yAngle);
        }
        //yAngle = -1*yAngle-90;
    }
          

    return Orientation{xAngle,yAngle};

};

Direction MotionDetection::getTiltDirection(uint tolerance){
    if (this->getAcceleration().z > 0){
        return Flipped;
    }
    Orientation Rot = this->getTilt();
    Serial.println(Rot.xRotation);
    Serial.println(Rot.xRotation == INT_MAX);
    if ((Rot.xRotation == INT_MAX)){
        return Error;
    }
    if(abs(abs(Rot.xRotation)-abs(Rot.yRotation))>tolerance){
        //determine which axis is more tiltet
        if (abs(Rot.xRotation)>abs(Rot.yRotation)){
            //test if dezibot is tilted left or right
            if (Rot.xRotation > 0){
                return Right;
            } else {
                return Left;
            }
        } else {
            //test if robot is tilted front or back
            if (Rot.yRotation > 0){
                return Front;
            } else {
                return Back;
            }
        }
    } else {
        //dezibot is (with tolerance) leveled
        return Neutral;
    }
};

uint8_t MotionDetection::cmdRead(uint8_t reg){
    return (CMD_READ | (reg & ADDR_MASK));
};
uint8_t MotionDetection::cmdWrite(uint8_t reg){
    return (CMD_WRITE | (reg & ADDR_MASK));
};

uint8_t MotionDetection::readRegister(uint8_t reg){
    handler->beginTransaction(SPISettings(frequency,SPI_MSBFIRST,SPI_MODE0));
    digitalWrite(34,LOW);
    uint8_t result;
    result = handler->transfer(cmdRead(reg));
    result = handler->transfer(0x00);
    digitalWrite(34,HIGH);
    handler->endTransaction(); 
    return result;
};

uint8_t MotionDetection::readFromRegisterBank(registerBank bank,uint8_t reg){
    uint8_t result = 0;
    switch(bank){
        case(MREG1):
            this->writeRegister(BLK_SEL_R,0x00);
            break;
        case(MREG2):
            this->writeRegister(BLK_SEL_R,0x28);
            break;
        case(MREG3):
            this->writeRegister(BLK_SEL_R,0x50);
            break;
    }
    this->writeRegister(MADDR_R,reg);
    delayMicroseconds(10);
    result=this->readRegister(M_R);
    delayMicroseconds(10);
    this->resetRegisterBankAccess();
    return result;
};

void MotionDetection::writeToRegisterBank(registerBank bank, uint8_t reg, uint8_t value){
    while((this->readRegister(MCLK_RDY))&0x08!=0x08){
        Serial.println("CLK not rdy");
        delay(100);
    }
    uint8_t result = this->readRegister(PWR_MGMT0);
    Serial.print("MADDR_W: ");
    Serial.println(readRegister(MADDR_W)); 
    //set Idle Bit
    this->writeRegister(PWR_MGMT0,result|0x10);
    switch(bank){
        case(MREG1):
           this->writeRegister(BLK_SEL_W,0x00);
            break;
        case(MREG2):
            this->writeRegister(BLK_SEL_W,0x28);
            break;
        case(MREG3):
            this->writeRegister(BLK_SEL_W,0x50);
            break;
    }
    this->writeRegister(MADDR_W,reg);
    this->writeRegister(M_W,value);
    delayMicroseconds(10);
    this->writeRegister(PWR_MGMT0,result&0xEF);
    Serial.print("MADDR_W: ");
    Serial.println(readRegister(MADDR_W));
    this->resetRegisterBankAccess();
};

void MotionDetection::resetRegisterBankAccess(){
    this->writeRegister(BLK_SEL_R,0x00);
    this->writeRegister(BLK_SEL_W,0x00);
    this->writeRegister(MADDR_R,0x00);
    this->writeRegister(MADDR_W,0x00);
};

void MotionDetection::initFIFO(){
    delay(60);
    //set INTF_CONFIG0 FIFO_COUNT_REC_RECORD und Little Endian
    this->writeRegister(INTF_CONFIG0,0x60);
    //set FIFO_CONFIG1 to Mode Snapshot and BYPASS Off
    this->writeRegister(FIFO_CONFIG1,0x00);
    //set TMST_CONFIG1_MREG1 TMST_CONFIIG1_TMST_EN
    this->writeToRegisterBank(MREG1,TMST_CONFIG1,0x00);
    //set FiFO config 5 GYRO_EN,TMST_FSYNC, ACCEL_EN, WM_GT_TH_EN
    this->writeToRegisterBank(MREG1,FIFO_CONFIG5,0x23);
    //set FOF_CONFIG2 0x1 (INT triggerd each packaged)
    this->writeRegister(FIFO_CONFIG2,0x0A);
};

uint MotionDetection::getDataFromFIFO(FIFO_Package* buffer){
    int16_t fifocount = 0;
    int8_t fifohigh = this->readRegister(FIFO_COUNTH);
    int8_t fifolow = this->readRegister(FIFO_COUNTL);  
    fifocount = (fifohigh<<8)|fifolow;
    //fifocount |= this->readRegister(FIFO_COUNTL);
    //fifocount = (this->readRegister(FIFO_COUNTH)<<8);
    Serial.println(fifolow);
    Serial.println(fifohigh);
    Serial.println(fifocount);
    handler->beginTransaction(SPISettings(frequency,SPI_MSBFIRST,SPI_MODE0));
    digitalWrite(34,LOW);
    handler->transfer(cmdRead(FIFO_DATA));
    handler->transfer(buf,16*fifocount);
    digitalWrite(34,HIGH);
    handler->endTransaction();
    
    writeRegister(0x02,0x04);
    delayMicroseconds(10);

    for(int i = 0; i<fifocount;i++){
        buffer[i].header = buf[0x00+16*i];
        buffer[i].accel.x = (buf[0x02+16*i]<<8)|buf[0x01+16*i]; 
        buffer[i].accel.y = (buf[0x04+16*i]<<8)|buf[0x03+16*i]; 
        buffer[i].accel.z = (buf[0x06+16*i]<<8)|buf[0x05+16*i]; 
        buffer[i].gyro.x = (buf[0x08+16*i]<<8)|buf[0x07+16*i]; 
        buffer[i].gyro.y = (buf[0x0A+16*i]<<8)|buf[0x09+16*i]; 
        buffer[i].gyro.z = (buf[0x0C+16*i]<<8)|buf[0x0B+16*i]; 
        buffer[i].temperature = buf[0x0D+16*i]; 
        buffer[i].timestamp = (buf[0x0F+16*i]<<8)|buf[0x0E +16*i]; 
    }
    return fifocount;
};

void MotionDetection::writeRegister(uint8_t reg, uint8_t value){
    handler->beginTransaction(SPISettings(frequency,SPI_MSBFIRST,SPI_MODE0));
    digitalWrite(34,LOW);
    handler->transfer(reg);
    handler->transfer(value);
    digitalWrite(34,HIGH);
    delayMicroseconds(10);
    handler->endTransaction();
};