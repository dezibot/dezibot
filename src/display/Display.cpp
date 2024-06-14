/**
 * @file Display.cpp
 * @author Hans Haupt (hans.haupt@dezibot.de)
 * @brief Adds the ability to print to the display of the robot.
 * @version 0.1
 * @date 2024-06-05
 * 
 * @copyright Copyright (c) 2024
 */

#include "Display.h"
#include "CharTable.h"
#include "Wire.h"


void Display::begin(void){
    //set Mux Ratio
    sendDisplayCMD(muxRatio);
    sendDisplayCMD(0x3f);
    sendDisplayCMD(setOffset);
    sendDisplayCMD(0x00);
    sendDisplayCMD(setStartLine);
    sendDisplayCMD(stopCompleteOn);
    /*which pixels are bright: normal = 1s are bright, inverese= 0s are bright*/
    sendDisplayCMD( setNormalMode);
 
    sendDisplayCMD( setOscFreq);
    sendDisplayCMD(0x80);

    sendDisplayCMD(setChargePump);
    sendDisplayCMD(0x14);
    sendDisplayCMD(activateDisplay);
    this->clear();
    return;
};

void Display::sendDisplayCMD(uint8_t cmd){
    Wire.beginTransmission(DisplayAdress);
    Wire.write(cmd_byte);
    Wire.write(cmd);
    Wire.endTransmission();
};

void Display::clear(void){
    sendDisplayCMD(addressingMode);
    sendDisplayCMD(0x00); //horizontal
    sendDisplayCMD(colRange);
    sendDisplayCMD(0x00);
    sendDisplayCMD(0x7f);
    sendDisplayCMD(pageRange);
    sendDisplayCMD(0x00);
    sendDisplayCMD(0x07);
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
        this->charsOnCurrLine = (charAmount+this->charsOnCurrLine)%17; //there can be 0-16 chars on one line, so the 17th char is on next line 
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
