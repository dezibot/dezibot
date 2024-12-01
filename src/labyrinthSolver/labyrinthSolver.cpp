// #include <Dezibot.h>
// #include <Wire.h>
// #include <veml6040.h>

// // Konstruktor
// LabyrinthSolver::LabyrinthSolver(Dezibot& dezibot) : dezibot(dezibot), TARGET_RED(0.0), TARGET_GREEN(0.0) {}

// // Setup-Methode
// void LabyrinthSolver::setup() {
    // Serial.begin(115200);
    // dezibot.begin();

    // Serial.println("Bitte Sensor über Gelb halten. Kalibrierung startet in 3 Sekunden...");
    // delay(3000);

    // calibrateYellow();

    // Serial.println("Kalibrierung abgeschlossen.");
    // Serial.print("TARGET_RED: ");
    // Serial.println(TARGET_RED, 2);
    // Serial.print("TARGET_GREEN: ");
    // Serial.println(TARGET_GREEN, 2);
// }

// // Haupt-Loop-Methode
// void LabyrinthSolver::loop() {
    // // Farbwerte in Prozent berechnen
    // double percentageRed, percentageGreen, percentageBlue;
    // getColorPercentages(percentageRed, percentageGreen, percentageBlue);

    // // Gelb erkennen basierend auf prozentualen Farbwerten
    // if (isYellowDetected(percentageRed, percentageGreen)) {
        // Serial.println("Gelb erkannt. Geradeaus fahren.");
        // dezibot.motion.move(0, 200);  // Geradeaus bewegen
    // } else {
        // Serial.println("Von Gelb abgekommen. Korrigiere Richtung.");

        // // Korrektur der Richtung
        // correctToYellow();
    // }

    // delay(50);  // Kleine Verzögerung zur Stabilität
// }

// // Kalibrierung der Gelbwerte
// void LabyrinthSolver::calibrateYellow() {
    // // Farbwerte erfassen und Zielwerte für Gelb berechnen
    // double percentageRed, percentageGreen, percentageBlue;
    // getColorPercentages(percentageRed, percentageGreen, percentageBlue);

    // TARGET_RED = percentageRed;
    // TARGET_GREEN = percentageGreen;
// }

// // Farbwerte in Prozent berechnen
// void LabyrinthSolver::getColorPercentages(double& percentageRed, double& percentageGreen, double& percentageBlue) {
    // uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED);
    // uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
    // uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
    // double sumColor = red + green + blue;

    // percentageRed = (red / sumColor) * 100.0;
    // percentageGreen = (green / sumColor) * 100.0;
    // percentageBlue = (blue / sumColor) * 100.0;
// }

// // Prüft, ob Gelb erkannt wurde
// bool LabyrinthSolver::isYellowDetected(double percentageRed, double percentageGreen) {
    // return abs(percentageRed - TARGET_RED) <= THRESHOLD &&
           // abs(percentageGreen - TARGET_GREEN) <= THRESHOLD;
// }

// // Korrektur der Richtung, um Gelb zu finden
// void LabyrinthSolver::correctToYellow() {
    // double percentageRed, percentageGreen, percentageBlue;

    // while (true) {
        // dezibot.motion.rotateAntiClockwise(200, 150);
        // delay(300);

        // getColorPercentages(percentageRed, percentageGreen, percentageBlue);
        // if (isYellowDetected(percentageRed, percentageGreen)) break;

        // dezibot.motion.rotateClockwise(200, 150);
        // delay(300);

        // getColorPercentages(percentageRed, percentageGreen, percentageBlue);
        // if (isYellowDetected(percentageRed, percentageGreen)) break;
    // }

    // Serial.println("Linie wiedergefunden. Zurück zur Linie.");
    // dezibot.motion.move(0, 200);  // Geradeaus bewegen
// }