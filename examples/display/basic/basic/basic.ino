#include "Dezibot.h"
#include "bitmaps.h"

Dezibot dezibot = Dezibot();

void setup() {
  // put your setup code here, to run once:
  dezibot.begin();
}

void loop() {
	// put your main code here, to run repeatedly:
	dezibot.display.print("Hello from\nDezibot!");
	delay(2000);
	dezibot.display.clear();
	dezibot.display.drawBitmap(0, 0, bitmap_c_logo, 64, 64);
	delay(2000);
	dezibot.display.clear();
	dezibot.display.playAnimation(0, 0, animation_frames, 6, 64, 64, 120);
	delay(2000);
	dezibot.display.clear();
	dezibot.display.print("Bye!");
	delay(2000);
	dezibot.display.clear();
  }
