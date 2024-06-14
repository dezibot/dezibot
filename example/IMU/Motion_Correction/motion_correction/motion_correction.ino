#include "Dezibot.h"
Dezibot dezibot = Dezibot();
#define maxDuty 6000
#define minDuty 3500
#define baseDuty 3900
void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
  Serial.begin(115200);
  //dezibot.motion.move();
  //analogWrite(11, 2000);
  //analogWrite(12, 2000);
}
uint16_t leftDuty= baseDuty;
uint16_t rightDuty = baseDuty;
unsigned long previous = millis();
unsigned long current = millis();
long averageZRotation = 0;
long averageXAccel = 0;
int left=0;
int right=0;
int difference = 0;
int changerate = 0;
int interval = 40;
int threshold =  150;
int resultcounter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  current = millis();
  IMUResult result = dezibot.motion.detection.getRotation();
  averageXAccel += dezibot.motion.detection.getAcceleration().x;
  averageZRotation += result.z;
  //Serial.println(result.z);
  if (result.z > threshold){
    right++;
  } else if(result.z < -threshold) {
    left++;
  }
  resultcounter++;
  if ((current - previous) > interval){
    //averageZRotation = averageZRotation / resultcounter;
    difference = abs(left-right);
    if (difference>25){
      changerate = 100;
    } else if(difference>20){
      changerate = 50;
    } else if(difference >15){
      changerate = 30;
    } else if(difference > 10){
      changerate = 10;
    } else{
      changerate = 5; 
    }
    //Serial.print("Z:");
    //Serial.println(averageZRotation);
    dezibot.display.clearDisplay();
    dezibot.display.setCursor(0, 0);
    dezibot.display.setTextColor(WHITE);
    dezibot.display.setTextSize(2); // Draw 2X-scale text
     dezibot.display.println(averageZRotation);
    //dezibot.display.println(resultcounter);
    dezibot.display.print(left);
    dezibot.display.print("   ");
    dezibot.display.println(right);
    if(left>right){ //rotates anticlock
      leftDuty+=changerate;
      rightDuty-=changerate;
    } else if(left<right){
      leftDuty-=changerate;
      rightDuty+=changerate;
    } 
    dezibot.motion.left.setSpeed(leftDuty);
    dezibot.motion.right.setSpeed(rightDuty);
    dezibot.display.println(leftDuty);
    dezibot.display.println(rightDuty);
    dezibot.display.display();


    averageZRotation = 0;
    averageXAccel = 0;
    resultcounter = 0;
    left = 0;
    right = 0;
    previous = current;
  }
}
