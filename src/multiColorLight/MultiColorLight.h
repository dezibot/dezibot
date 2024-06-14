/**
 * @file MultiColorLight.h
 * @author Saskia Duebener, Hans Haupt
 * @brief This component controls the ability to show multicolored light, using the RGB-LEDs
 * @version 0.2
 * @date 2023-11-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MultiColorLight_h
#define MultiColorLight_h
#include <Adafruit_NeoPixel.h>
#include "ColorConstants.h"
/**
 * @brief Describes combinations of leds on the Dezibot. 
 * With the Robot in Front of you, when the robot drives away from you, the left LED is TOP_LEFT 
 * 
 */
enum leds{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM,
    TOP,
    ALL
};

class MultiColorLight{
protected:
    static const uint16_t ledAmount = 3;
    static const int16_t ledPin = 48;
    static const uint8_t maxBrightness = 150;
    Adafruit_NeoPixel rgbLeds;
public:
    
    MultiColorLight();
    /**
     * @brief initialize the multicolor component
     * 
     */
    void begin(void);

    /**
     * @brief Set the specified led to the passed color
     * @param index ranging from 0-2, 0: Right, 1: Left, 2: Bottom 
     * @param color A 32-bit unsigned integer representing the color in the format
    *               0x00RRGGBB, where RR is the red component, GG is the green
    *               component, and BB is the blue component. Each color can range between 0 to 100
     */
    void setLed(uint8_t index , uint32_t color);

    /**
     * @brief Set the specified leds to the passed color value
     * 
     * @param leds which leds should be updated
     * @param color A 32-bit unsigned integer representing the color in the format
    *               0x00RRGGBB, where RR is the red component, GG is the green
    *               component, and BB is the blue component. Each color can range between 0 to 100
     */
    void setLed(leds leds, uint32_t color);

    /**
     * @brief Set the specified leds to the passed color value
     * 
     * @param leds which leds should be updated
     * @param red brightness of red, is normalized in the function
     * @param green brightness of green, is normalized in the function
     * @param blue brightness of blue, is normalized in the function
     */
    void setLed(leds leds, uint8_t red, uint8_t green, uint8_t blue);
    
    /**
     * @brief sets the two leds on the top of the robot to the specified color
     * 
     * @param color A 32-bit unsigned integer representing the color in the format
    *               0x00RRGGBB, where RR is the red component, GG is the green
    *               component, and BB is the blue component. Each color can range between 0 to 100
     */
    void setTopLeds(uint32_t color);    

    /**
     * @brief sets the two leds on the top of the robot to the specified color
     * 
     * @param red brightness of red, is normalized in the function
     * @param green brightness of green, is normalized in the function
     * @param blue brightness of blue, is normalized in the function
     */
    void setTopLeds(uint8_t red, uint8_t green, uint8_t blue);  
    
    /**
     * @brief Let LEDs blink, returns after all blinks were executed
     * 
     * @param amount how often should the leds blink
     * @param color A 32-bit unsigned integer representing the color in the format
    *               0x00RRGGBB, where RR is the red component, GG is the green
    *               component, and BB is the blue component.
    *               Each color can range between 0 to 100
    *               Defaults to blue
     * @param leds which LEDs should blink, default is TOP
     * @param interval how many miliseconds the led is on, defaults to 1s
     */
    void blink(uint16_t amount,uint32_t color = 0x00006400,leds leds=TOP, uint32_t interval=1000);

    /**
     * @brief turn off the given leds
     * 
     * @param leds which leds should be turned off, defaults to ALL
     */
    void turnOffLed(leds leds=ALL);
    
    /**
     * @brief wrapper to calulate the used colorformat from a rgb-value
     * 
     * @param r red (0-100)
     * @param g green (0-100)
     * @param b blue (0-100)
     * @return A 32-bit unsigned integer representing the color in the format
    *          0x00RRGGBB, where RR is the red component, GG is the green
    *          component, and BB is the blue component.
     */
    uint32_t color(uint8_t r, uint8_t g, uint8_t b);

private:
    /**
     * @brief normalizes every component of color to not exeed the maxBrightness
     * 
     * @param color A 32-bit unsigned integer representing the color in the format
        *               0x00RRGGBB, where RR is the red component, GG is the green
        *               component, and BB is the blue component.
    * @param maxBrigthness maximal level of brightness that is allowed for each color
    * @return uint32_t A 32-bit unsigned integer representing the color in the format
        *               0x00RRGGBB, where RR is the red component, GG is the green
        *               component, and BB is the blue component. Where each component can be
        *               between 0 - maxBrightness
    */
    uint32_t normalizeColor(uint32_t color, uint8_t maxBrigthness=maxBrightness);
};

#endif //MultiColorLight_h