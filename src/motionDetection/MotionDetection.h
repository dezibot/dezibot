/**
 * @file MotionDetection.h
 * @author Hans Haupt
 * @brief This component controls the IMU (Accelerometer & Gyroscope) ICM-42670-P
 * @version 0.1
 * @date 2023-12-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MotionDetection_h
#define MotionDetection_h
#include <SPI.h>
#include <Arduino.h>
#include "IMU_CMDs.h"
struct IMUResult{
    int16_t x;
    int16_t y;
    int16_t z;
};
enum Axis{
    xAxis = 0x01,
    yAxis = 0x02,
    zAxis = 0x04
};

struct Orientation{
    int xRotation;
    int yRotation;
};

enum Direction{
    Front,
    Left,
    Right,
    Back,
    Neutral,
    Flipped,
    Error
};

struct FIFO_Package{
    int8_t header;
    IMUResult gyro;
    IMUResult accel;
    int16_t temperature;
    int16_t timestamp;
};


class MotionDetection{
protected:
    enum registerBank{MREG1,MREG2,MREG3};
    static const uint frequency = 24000000;
    static const uint16_t defaultShakeThreshold = 500;
    const uint bufferLength = 64*16;
    int8_t* buf = new int8_t[bufferLength];
    uint8_t readFromRegisterBank(registerBank bank,uint8_t reg);
    void writeToRegisterBank(registerBank bank, uint8_t reg, uint8_t value);
    void resetRegisterBankAccess();

    uint16_t cmdRead(uint8_t regHigh,uint8_t regLow);
    uint16_t cmdWrite(uint8_t regHigh,uint8_t regLow);
    uint8_t cmdRead(uint8_t reg);
    uint8_t cmdWrite(uint8_t reg);

    uint8_t readRegister(uint8_t reg);
    int16_t readDoubleRegister(uint8_t lowerReg);
    void writeRegister(uint8_t reg, uint8_t value);
    void initFIFO();

    SPIClass * handler = NULL;

    uint gForceCalib = 4050;
     
    
public:
    MotionDetection();

    /**
     * @brief initialized the  IMU Component. 
     * Wakes the IMU from Standby 
     * Set configuration 
     * 
     */
    void begin(void);

    /**
     * @brief stops the component
     * Sets the IMU to Low-Power-Mode
     * 
     */
    void end(void);

    /**
     * @brief Triggers a new Reading of the accelerationvalues and reads them from the IMU
     * 
     * @return IMUResult that contains the new read values
     */
    IMUResult getAcceleration(void);

    /**
     * @brief Triggers a new reading of the gyroscope and reads the values from the imu
     * 
     * @return IMUResult 
     */
    IMUResult getRotation(void);

    /**
     * @brief Reads the current On Chip temperature of the IMU 
     * 
     * @return normalized temperature in degree Centigrade
     */
    float getTemperature(void);

    /**
     * @brief Returns the value of reading the whoAmI register
     * When IMU working correctly, value should be 0x67
     * 
     * @return the value of the whoami register of the ICM-42670 
     */
    int8_t getWhoAmI(void);

    /**
     * @brief Detects if at the time of calling is shaken. Therefore the sum over all accelerationvalues is calculated 
     * and checked against threshold. If sum > threshold a shake is detected, else not
     * 
     * @param threshold (optional) the level of acceleration that must be reached to detect a shake
     * @param axis (optional) select which axis should be used for detection. Possible values ar xAxis,yAxis,zAxis
     * It's possible to combine multiple axis with the bitwise or Operator | 
     * For Example: to detect x and y axis: axis = xAxis|yAxis
     * 
     * @return true if a shake is detected, false else
    */
    bool isShaken(uint32_t threshold = defaultShakeThreshold,uint8_t axis = xAxis|yAxis|zAxis);

    /**
     * @brief calculates how the robot is tilted. It is set, that when the robot is placed normally on a flat table, the result will be (0,0)
     * Tilting the robot, so that the front leg is deeper than the other to results in an increasing degrees, tilting the front leg up will increase negativ degrees
     * Tilting the robot to the right will increase the degrees until 180° (upside down), tilting it left will result in increasing negativ degrees (-1,-2,...,-180).
     * On the top there is a jump of the values from 180->-180 and vice versa.
     * 
     * Precision is rounded to 1 deg steps
     * 
     * @attention The results are only valid, if the robot is not moved in any way during the measurment, as the calculation is made by using the accelration values.
     * If it's detected, that the robot is accelerated while measuring, the method will return max(int).
     * Please note that the imu is pretty sensitiv, even walking next to the table may influcene the result. 
     * 
    */
    Orientation getTilt();


    /**
     * @brief Checks in which direction (Front, Left, Right, Back) the robot is tilted. 
     * 
     * @attention Does only work if the robot is not moving
     *  
     * @param tolerance (optional, default = 10) how many degrees can the robot be tilted, and still will be considerd as neutral. 
     * 
     * 
     * @return Direction the direction in that the robot is tilted most. Front is onsiderd as the direction of driving.
     * If robot is not tilted more than the tolerance in any direction, return is Neutral.
     * If Robot is upside down, return is Flipped.
     * If Robot is moved, return is Error
     */
    Direction getTiltDirection(uint tolerance = 10);

    /**
     * can be used to set a custom value for the gforceReading of the zaxis, which will improve the getTiltFunction.
     * 
     * @attention this method is not persisten, so the value is not stored when the programm is restarted / the robot is powerd off
     * 
     * @param gforceValue the value the IMU returns for the gravitationforce -> to get this value, place the robot on a leveled surface
     * and read the value getAcceleration().z
    */
    void calibrateZAxis(uint gforceValue);

    /**
     * @brief will read all availible packages from fifo, after 40ms Fifo is full
     * 
     * @param buffer pointer to FIFO_Package Struct that at least must have size 64 (this is the max package count with APEX Enabled)
     * 
     * @return the amount of acutally fetched packages 
    */
    uint getDataFromFIFO(FIFO_Package* buffer);
};
#endif //MotionDetection