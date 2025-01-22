#include <Dezibot.h>
#include <Wifi.h>

Dezibot dezibot = Dezibot();

void setup() {
    // put your setup code here, to run once:
    dezibot.begin();

    dezibot.display.print("SETUP");
    // dezibot.motion.move(1000);
    delay(5000);
    dezibot.display.clear();
}

void loop() {
    // put your main code here, to run repeatedly:
    // dezibot.display.print("Hello from\nDezibot!");
    // dezibot.motion.move(1000);
    // delay(5000);
    // dezibot.display.clear();

    dezibot.display.print("TESTING");
    // dezibot.motion.move(1000);
    delay(5000);
    dezibot.display.clear();

}
