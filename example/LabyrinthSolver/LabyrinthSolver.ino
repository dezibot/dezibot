// #include <Dezibot.h>

// Dezibot dezibot = Dezibot();

// double TARGET_RED;
// double TARGET_GREEN;

// const double THRESHOLD = 0.15;
// const double BASE_SPEED = 3900;

// void setup() {
//   Serial.begin(115200);
//   dezibot.begin();
//   dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);
//   displayMessage("Kalibrierung");
//   delay(3000);
//   calibrateYellow();
// }

// void loop() {
//   dezibot.display.clear();
//   double percentageRed, percentageGreen, percentageBlue;
//   getColorPercentages(percentageRed, percentageGreen, percentageBlue);

//   dezibot.display.println(String(percentageRed, 2) + " " + String(percentageGreen, 2));
//   dezibot.display.println(String(abs(percentageRed - TARGET_RED), 2) + " " + String(abs(percentageGreen - TARGET_GREEN), 2));
//   delay(5000);

//   if (abs(percentageRed - TARGET_RED) <= THRESHOLD && abs(percentageGreen - TARGET_GREEN) <= THRESHOLD) {
//     displayMessage("Gelb erkannt. Geradeaus fahren.");
//     dezibot.motion.left.setSpeed(BASE_SPEED);
//     dezibot.motion.right.setSpeed(BASE_SPEED);
//   } else {
//     displayMessage("Von Gelb abgekommen. Korrigiere Richtung.");
//     double initialRed = percentageRed;
//     double initialGreen = percentageGreen;

//     // Kurz nach links drehen
//     dezibot.motion.left.setSpeed(0);
//     dezibot.motion.right.setSpeed(BASE_SPEED);
//     delay(500);
//     getColorPercentages(percentageRed, percentageGreen, percentageBlue);

//     if (percentageRed < initialRed || percentageGreen < initialGreen) {
//       displayMessage("GEHE RECHTS.");
//       dezibot.motion.left.setSpeed(BASE_SPEED);
//       dezibot.motion.right.setSpeed(0);
//     } else {
//       displayMessage("GEHE LINKS.");
//       dezibot.motion.left.setSpeed(0);
//       dezibot.motion.right.setSpeed(BASE_SPEED);
//     }

//     while (true) {
//       getColorPercentages(percentageRed, percentageGreen, percentageBlue);
//       if (abs(percentageRed - TARGET_RED) < THRESHOLD && abs(percentageGreen - TARGET_GREEN) < THRESHOLD) {
//         displayMessage("Linie gefunden");
//         break;
//       }
//     }

//     displayMessage("Linie wiedergefunden. Weiter geradeaus.");
//     dezibot.motion.left.setSpeed(BASE_SPEED);
//     dezibot.motion.right.setSpeed(BASE_SPEED);
//   }
// }

// void calibrateYellow() {
//   double percentageRed, percentageGreen, percentageBlue;
//   getColorPercentages(percentageRed, percentageGreen, percentageBlue);

//   TARGET_RED = percentageRed;
//   TARGET_GREEN = percentageGreen;
// }

// void getColorPercentages(double &percentageRed, double &percentageGreen, double &percentageBlue) {
//   uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED);
//   uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
//   uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
//   double sumColor = red + green + blue;

//   percentageRed = (red / sumColor) * 100.0;
//   percentageGreen = (green / sumColor) * 100.0;
//   percentageBlue = (blue / sumColor) * 100.0;
// }

// void displayMessage(const char *message) {
//   dezibot.display.clear();
//   dezibot.display.println(message);
//   Serial.println(message);
// }