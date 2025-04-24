#include <Dezibot.h>  
Dezibot dezibot = Dezibot(); 
void setup() {   
    dezibot.begin(); 
}  
void loop() {   
  for (int i=1; i<=10; i++){
    dezibot.display.clear();
    dezibot.display.print(i);
    delay(1000);
  }

}

