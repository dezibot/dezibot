/**
 * @file Display.cpp
 * @author Hans Haupt (hans.haupt@dezibot.de), Bastian Wecke (bastian.wecke@stud.htwk-leipzig.de)
 * @brief Adds the ability to print to the display of the robot.
 * @version 0.1
 * @date 2024-06-05
 * 
 * @copyright Copyright (c) 2024
 */

#include "Display.h"
#include "CharTable.h"
#include "Wire.h"
#include <pgmspace.h>


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

void Display::sendBitmapRegion(uint8_t x, uint8_t y, const uint8_t* data, uint8_t w, uint8_t h, bool fill) {
    if (!fill && !data) return;
    if (w == 0 || h == 0) return;
    if ((y % 8) != 0 || (h % 8) != 0) return;
    if (x > 127 || y > 63) return;

    uint8_t maxW = 128 - x;
    uint8_t maxH = 64 - y;
    uint8_t drawW = (w < maxW) ? w : maxW;
    uint8_t drawH = (h < maxH) ? h : maxH;
    drawH -= (drawH % 8);
    if (drawW == 0 || drawH == 0) return;

    uint8_t startPage = y / 8;
    uint8_t endPage = startPage + (drawH / 8) - 1;

    sendDisplayCMD(addressingMode);
    sendDisplayCMD(0x00);
    sendDisplayCMD(colRange);
    sendDisplayCMD(x);
    sendDisplayCMD(x + drawW - 1);
    sendDisplayCMD(pageRange);
    sendDisplayCMD(startPage);
    sendDisplayCMD(endPage);

    uint16_t total = (uint16_t)drawW * (drawH / 8);
    uint16_t idx = 0;
    while (idx < total) {
        Wire.beginTransmission(DisplayAdress);
        Wire.write(data_byte);
        for (uint8_t chunk = 0; chunk < 16 && idx < total; ++chunk, ++idx) {
            Wire.write(fill ? static_cast<uint8_t>(0x00)
                            : pgm_read_byte(data + idx));
        }
        Wire.endTransmission();
    }
}

void Display::drawBitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t w, uint8_t h) {
    sendBitmapRegion(x, y, bitmap, w, h, false);
}

void Display::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    sendBitmapRegion(x, y, nullptr, w, h, true);
}

void Display::setBrightness(uint8_t value) {
    sendDisplayCMD(setContrast);
    sendDisplayCMD(value);
}

void Display::playAnimation(uint8_t x, uint8_t y, const uint8_t* frames, uint8_t frameCount, uint8_t w, uint8_t h, uint16_t frameDurationMs) {
    if (!frames || frameCount == 0 || w == 0 || h == 0) return;
    if ((y % 8) != 0 || (h % 8) != 0) return;

    uint16_t frameSize = (uint16_t)w * (h / 8);
    for (uint8_t i = 0; i < frameCount; i++) {
        uint32_t start = millis();
        drawBitmap(x, y, frames + (uint32_t)i * frameSize, w, h);
        uint32_t elapsed = millis() - start;
        if (frameDurationMs > elapsed) delay(frameDurationMs - elapsed);
    }
}

void Display::playAnimation(uint8_t x, uint8_t y, const uint8_t* const* framePtrs, uint8_t frameCount, uint8_t w, uint8_t h, uint16_t frameDurationMs) {
    if (!framePtrs || frameCount == 0 || w == 0 || h == 0) return;
    if ((y % 8) != 0 || (h % 8) != 0) return;

    for (uint8_t i = 0; i < frameCount; i++) {
        const uint8_t* frame = framePtrs[i];
        if (!frame) continue;
        uint32_t start = millis();
        drawBitmap(x, y, frame, w, h);
        uint32_t elapsed = millis() - start;
        if (frameDurationMs > elapsed) delay(frameDurationMs - elapsed);
    }
}
