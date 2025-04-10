#include <Dezibot.h>  
Dezibot dezibot = Dezibot(); 
void setup() {   
    dezibot.begin();
}  
void loop() {   
    dezibot.motion.move(1000);   
    dezibot.multiColorLight.setLed(BOTTOM, RED);   
    delay(2000);   
    dezibot.motion.rotateAntiClockwise(1000);   
    dezibot.multiColorLight.setLed(BOTTOM, GREEN);   
    delay(2000);   
    dezibot.motion.rotateClockwise(1000);   
    dezibot.multiColorLight.setLed(BOTTOM, BLUE);   
    delay(2000);   
    dezibot.multiColorLight.turnOffLed();   
    delay(2000); 
}
