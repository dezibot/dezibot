/****************************************************
* If you have selected ESP32-S3-USB_OTG as the board, 
* change Tools->USB_Mode to "Hardware CDC and JTAG".
*****************************************************/

#include <Dezibot.h>

Dezibot dezibot = Dezibot();

void setup() {
  Serial.begin(115200);
  Serial.println("Started");
  dezibot.begin();
  Serial.println("Inited");
}

void loop() {
  Serial.println(dezibot.lightDetection.getValue(DL_FRONT));
  delay(1000);
}

