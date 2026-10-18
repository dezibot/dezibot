/**
 * @file Display.h
 * @author Hans Haupt (hans.haupt@dezibot.de), Bastian Wecke (bastian.wecke@stud.htwk-leipzig.de)
 * @brief Adds the ability to print to the display of the robot.
 * @version 0.1
 * @date 2024-05-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef Display_h
#define Display_h
#include <stdint.h>
#include <Arduino.h>
#include "DisplayCMDs.h"

class Display{
    protected:
        //how many chars are on current line
        uint8_t charsOnCurrLine = 0;
        
        //on which line are we currently printing
        uint8_t currLine = 0;
        
        //flag that marks if the y-orientation is currently flipped
        bool orientationFlipped = false;

        //flag thats marks if the color is currently inverted
        bool colorInverted = false;
        
        /**
         * @brief sends the passed cmd to the display, cmd_byte is added as prefix by the function
         * 
         * @param cmd the byte instruction that shold by sent 
         */
        void sendDisplayCMD(uint8_t cmd);

        /**
         * @brief should be called whenever characters where printed to the display.
         * Updates the data of the class to handle linebreaks correctly
         * @param charAmount How many characters where added to the screen
         */
        void updateLine(uint charAmount);

        /**
         * @brief shared implementation for drawBitmap and fillRect.
         * Validates, clips, sets up the display region, and streams bytes via I2C.
         * Uses pgm_read_byte which works transparently for both RAM and PROGMEM on ESP32.
         * @param x column start (0–127)
         * @param y row start (0–63, must be multiple of 8)
         * @param data pointer to bitmap data (ignored when fill=true)
         * @param w width in pixels
         * @param h height in pixels (must be multiple of 8)
         * @param fill if true, writes 0x00 instead of reading from data
         */
        void sendBitmapRegion(uint8_t x, uint8_t y, const uint8_t* data, uint8_t w, uint8_t h, bool fill);

    public:
        
        /**
         * @brief initializes the display datastructures and sents the required cmds to start the display. Should only be called once.
         * @warning doesn't initalize the I²C bus itself, therefore wire.begin(1,2) must be called elsewhere, before this method.
        */
        void begin(void);
        
        /**
         * @brief delets all content from the display, resets the linecounter, new print will start at the top left. 
         *      Orientationflip is not resetted
        */
        void clear(void);
        
        /**
         * @brief prints the passed string right behind the current displaycontent
         * the sequence "\n" can be used to make a linebreak on the display
         * 
         * @param value the string "xyz" that should be printed to the display
        */
        void print(char *value);

        /**
         * @brief same as the print method, but after the string a line break is inserted
         * 
         * @param value the string that should be printed
        */
        void println(char *value);
        
        /**
         * @brief prints the passed string right behind the current displaycontent
         * the sequence "\n" can be used to make a linebreak on the display
         * 
         * @param value the string "xyz" that should be printed to the display
        */
        void print(String value);

        /**
         * @brief same as the print method, but after the string a line break is inserted
         * 
         * @param value the string that should be printed
        */
        void println(String value);
        
        /**
         * @brief prints the passed string right behind the current displaycontent
         * the sequence "\n" can be used to make a linebreak on the display
         * 
         * @param value the string "xyz" that should be printed to the display
        */
        void print(int value);

        /**
         * @brief same as the print method, but after the string a line break is inserted
         * 
         * @param value the string that should be printed
        */
        void println(int value);

        /**
         * @brief string to char
         * 
         * @param value the string that should be converted to char
        */
        char stringToCharArray(String value);


        /**
         * @brief flips the horizontal orientation of all content on the display
        */
        void flipOrientation(void);

        /**
         * @brief inverts the pixelcolors, so pixels on will be set to off and currently off pixels will be turned off.
         * affects already printed content as well as future prints.
         * 
         */
        void invertColor(void);

        /**
         * @brief draws a monochrome bitmap at (x,y). Requires page alignment: y and h must be multiples of 8.
         * Bitmap format: SSD1306 column-major, each byte = 8 vertical pixels.
         * Works with both RAM and PROGMEM data (pgm_read_byte is a no-op on ESP32).
         * @param x column start (0–127)
         * @param y row start (0–63, must be multiple of 8)
         * @param bitmap pointer to bitmap data (RAM or PROGMEM)
         * @param w width in pixels
         * @param h height in pixels (must be multiple of 8)
         */
        void drawBitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t w, uint8_t h);

        /**
         * @brief fills a rectangle with black (erases region). Requires page alignment: y and h multiples of 8.
         * Use for partial updates (e.g. erase before redrawing a sprite) to avoid full-screen flicker.
         * @param x column start (0–127)
         * @param y row start (0–63, must be multiple of 8)
         * @param w width in pixels
         * @param h height in pixels (must be multiple of 8)
         */
        void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

        /**
         * @brief sets display brightness (contrast). 0 = dimmest, 255 = brightest.
         * @param value brightness level (0–255)
         */
        void setBrightness(uint8_t value);

        /**
         * @brief plays a frame animation at (x,y). Frames must be in PROGMEM, concatenated.
         * Each frame overwrites the previous. Blocks for the full duration.
         * @param x column start
         * @param y row start (must be multiple of 8)
         * @param frames pointer to PROGMEM data (frame0, frame1, frame2, ... concatenated)
         * @param frameCount number of frames
         * @param w width of each frame
         * @param h height of each frame (must be multiple of 8)
         * @param frameDurationMs milliseconds per frame
         */
        void playAnimation(uint8_t x, uint8_t y, const uint8_t* frames, uint8_t frameCount, uint8_t w, uint8_t h, uint16_t frameDurationMs);

        /**
         * @brief plays a frame animation from an array of frame pointers (e.g. image2cpp allArray).
         * Each frame must be in PROGMEM. Use this overload when frames are separate arrays.
         * Each frame overwrites the previous. Blocks for the full duration.
         * @param x column start
         * @param y row start (must be multiple of 8)
         * @param framePtrs array of pointers to PROGMEM frame data
         * @param frameCount number of frames
         * @param w width of each frame
         * @param h height of each frame (must be multiple of 8)
         * @param frameDurationMs milliseconds per frame
         */
        void playAnimation(uint8_t x, uint8_t y, const uint8_t* const* framePtrs, uint8_t frameCount, uint8_t w, uint8_t h, uint16_t frameDurationMs);
};


#endif //Display_h