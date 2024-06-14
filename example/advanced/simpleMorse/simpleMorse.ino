#include "Dezibot.h"

Dezibot dezibot = Dezibot();
void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  dezibot.display.print("Starte Nachricht!");
  delay(2000);
  dezibot.display.clear();

  // T
  longSignal(1);
  pauseLetter();
  // E
  shortSignal(1);
  pauseLetter();
  // S
  shortSignal(3);
  pauseLetter();
  // T
  longSignal(1);
  pauseLetter();

  pauseWord();

  //A
  shortSignal(1);
  longSignal(1);
  pauseLetter();
  //B
  longSignal(1);
  shortSignal(3);
  pauseLetter();
  //C
  longSignal(1);
  shortSignal(1);
  longSignal(1);
  shortSignal(1);

  dezibot.display.print("Ende Nachricht!");
  delay(2000);
  dezibot.display.clear();
}

void shortSignal(int count){
  for(int i=0; i < count; i++){
    dezibot.motion.move();
    delay(1000);
    dezibot.motion.stop();
    delay(1000);
  }
}

void longSignal(int count){
  for(int i=0; i < count; i++){
    dezibot.motion.move();
    delay(3000);
    dezibot.motion.stop();
    delay(1000);
  }
}

void pauseLetter(){
  delay(2000);
}

void pauseWord(){
  delay(6000);
}