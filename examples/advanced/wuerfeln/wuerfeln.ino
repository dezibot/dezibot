#include <Dezibot.h>  
Dezibot dezibot = Dezibot(); 
void setup() {   
    dezibot.begin(); 
}  
void loop() {   
  if(dezibot.motion.detection.isShaken()){
    dezibot.display.clear();
    dezibot.display.print(random(1,7));
    delay(1000);
  }

}

