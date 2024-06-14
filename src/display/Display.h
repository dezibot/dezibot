/**
 * @file InfraredLight.h
 * @author Hans Haupt (hans.haupt@dezibot.de)
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
};


#endif //Display_h