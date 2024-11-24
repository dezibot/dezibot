/**
 * @file Display.cpp
 * @author Hans Haupt (hans.haupt@dezibot.de), Florian Schmidt
 * @brief Adds the ability to print to the display of the robot.
 * @version 0.2
 * @date 2024-11-24
 * 
 * @copyright Copyright (c) 2024
 */

#include "Display.h"
#include "CharTable.h"
#include "Wire.h"


void Display::begin(void){
    uint8_t cmds[] = {
        //set Mux Ratio
        muxRatio,
        0x3f,
        setOffset,
        0x00,
        setStartLine,
        stopCompleteOn,
        /*which pixels are bright: normal = 1s are bright, inverese= 0s are bright*/
        setNormalMode,

        setOscFreq,
        0x80,

        setChargePump,
        0x14,
        activateDisplay
    };
    sendDisplayCMDs(cmds, sizeof(cmds));

    this->clear();
    return;
};

void Display::sendDisplayCMD(uint8_t cmd){
    Wire.beginTransmission(DisplayAdress);
    Wire.write(cmd_byte);
    Wire.write(cmd);
    Wire.endTransmission();
};

void Display::sendDisplayCMDs(uint8_t *cmds, size_t cmd_count){
  Wire.beginTransmission(DisplayAdress);
  Wire.write(0x00); // continuation bit
  for (size_t i = 0; i < cmd_count; i++)
  {
    Wire.write(cmds[i]);
  }
  Wire.endTransmission();
};

void Display::clear(void){ 
    uint8_t cmds[] = {
        addressingMode,
        0x00,
        colRange,
        0x00,
        0x7f,
        pageRange,
        0x00,
        0x07
    };
    sendDisplayCMDs(cmds, sizeof(cmds));
    for (int j=0;j<64;j++){
        Wire.beginTransmission(DisplayAdress);
        Wire.write(data_byte);
        for(int i = 0;i<16;i++){
                    Wire.write(0x00);
        }
        Wire.endTransmission();
    }
    this -> charsOnCurrLine = 0;
    this -> currLine = 0;
    return;
};

void Display::updateLine(uint charAmount)
{
    if(charAmount+this->charsOnCurrLine>16)
    {
        this->currLine = (this->currLine+((charAmount+this->charsOnCurrLine)/16))%8;
        this->charsOnCurrLine = (charAmount+this->charsOnCurrLine)%16; //there can be 0-16 chars on one line, so the 17th char is on next line 
    }
    else
    {
        this->charsOnCurrLine = charAmount+this->charsOnCurrLine;
    }
};

void Display::print(char *value){
    char *nextchar;
	/* write data to the buffer */
    while(value && *value != '\0') //check if pointer is still valid and string is not terminated 
        {
	        //check if next character is a linebreak
            if(*value=='\n')
            {
                //fill the current line with blanks
                while(this->charsOnCurrLine<16)
                {
                    updateLine(1);
                    Wire.beginTransmission(DisplayAdress);
                    for(int i = 0;i<9;i++){
                        Wire.write(font8x8_colwise[0][i]);
                    }
                    Wire.endTransmission();
                }
                //make the linebreak
                this->currLine=currLine+1;
                this->charsOnCurrLine=0;
            }
            else
            {
                updateLine(1);
                Wire.beginTransmission(DisplayAdress);
                //print the character
                for(int i = 0;i<9;i++){
                            Wire.write(font8x8_colwise[*value][i]);
                }
                Wire.endTransmission();
            }
            value++;     
        }
};

char Display::stringToCharArray(String value) {
  const int len = value.length() + 1;  // +1 for the null terminator
  char msgBuffer[len];               // Create a buffer of the appropriate length

  value.toCharArray(msgBuffer, len);  // Copy the string into the buffer, including the null terminator

  return *msgBuffer;
}

void Display::print(String value){
    const int len = value.length() + 1;  // +1 for the null terminator
  char msgBuffer[len];               // Create a buffer of the appropriate length
  value.toCharArray(msgBuffer, len);

  this->print(msgBuffer);
};

void Display::println(String value){
  const int len = value.length() + 1;  // +1 for the null terminator
  char msgBuffer[len];               // Create a buffer of the appropriate length
  value.toCharArray(msgBuffer, len);

  this->println(msgBuffer);
};

void Display::print(int value){
  char cstr[16];

  this->print(itoa(value, cstr, 10));
};

void Display::println(int value){
  char cstr[16];

  this->println(itoa(value, cstr, 10));
};

void Display::println(char *value){
    this ->print(value);
    this->print("\n");
};

void Display::flipOrientation(void){
    if(this->orientationFlipped){
        sendDisplayCMD(setComDirectionNormal);
        sendDisplayCMD(setSegmentMap);
    } else{
        sendDisplayCMD(setComDirectionFlipped);
        sendDisplayCMD(setSegmentReMap);
    }
    this->orientationFlipped = !this->orientationFlipped;
};

void Display::invertColor(void){
    if(this->colorInverted){
        sendDisplayCMD(setNormalMode);
    } else {
        sendDisplayCMD(setInverseMode);
    }
    this->colorInverted = !this->colorInverted;
};

void Display::displayBattery(uint8_t batteryLevel, BatteryLocation location){
    uint8_t startx = (location == BatteryLocation::TOP_LEFT || location == BatteryLocation::BOTTOM_LEFT) ? 0 : 112;
    uint8_t starty = (location == BatteryLocation::TOP_LEFT || location == BatteryLocation::TOP_RIGHT) ? 0 : 7;

    // 8 control bytes
    uint8_t ctrl_cmds[8] = {
        addressingMode,
        0x00,
        colRange,
        startx,
        startx+15,
        pageRange,
        starty,
        starty,
    };
    sendDisplayCMDs(ctrl_cmds, sizeof(ctrl_cmds));

    // 16 data bytes (16 cols*1 page)
    uint8_t data_cmds[16];

    // essentially round(battLevel * 12 / 100) without floating point arithmetic
    const uint8_t battSegments = 12;
    uint16_t temp = (uint16_t)batteryLevel * battSegments;
    uint8_t upperSegment = (temp + 50) / 100;

    data_cmds[0] = B01111110;
    for (int i = 1; i < battSegments + 1; i++)
    {
        data_cmds[i] = i <= upperSegment ? 0xff : B10000001;
    };
    data_cmds[13] = 0xff;
    data_cmds[14] = data_cmds[15] = B00111100;

    Wire.beginTransmission(DisplayAdress);
    Wire.write(data_byte);
    for (int i = 0; i < sizeof(data_cmds); i++)
    {
        Wire.write(data_cmds[i]);
    };
    Wire.endTransmission();
};
