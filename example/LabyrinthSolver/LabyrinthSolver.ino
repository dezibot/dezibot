#include <Dezibot.h>

LabyrinthConfig config;
Dezibot dezibot = Dezibot();

void setup() {
    Serial.begin(115200);
    dezibot.begin();
    dezibot.multiColorLight.setLed(BOTTOM, 100, 100, 100);
    dezibot.labyrinthMovement.setColorMode(GREEN_LEFT);
    dezibot.labyrinthMovement.calibrateWhite();
}

void loop() {
    double percentageRed, percentageGreen, percentageBlue;
    dezibot.labyrinthMovement.getColorPercentages(percentageRed, percentageGreen, percentageBlue);

    // //DEBUG_COLOR_CALIBRATION_BEGIN
    // dezibot.display.println(String(percentageRed,2));
    // dezibot.display.println(String(percentageGreen,2));
    // delay(4000);
    // dezibot.display.clear();
    // //DEBUG_COLOR_CALIBRATION_END

    dezibot.labyrinthMovement.setIsOnWhite(
        dezibot.labyrinthMovement.isColorCloseTo(dezibot.labyrinthMovement.getCalibratedRed(), percentageRed, config.getWhiteTolerance()) &&
        dezibot.labyrinthMovement.isColorCloseTo(dezibot.labyrinthMovement.getCalibratedGreen(), percentageGreen, config.getWhiteTolerance())
    );

    if (dezibot.labyrinthMovement.getIsOnWhite()) {
        dezibot.labyrinthMovement.deadEndRotation();
    }

    bool isFirstGreater = dezibot.labyrinthMovement.compareColors(percentageRed, percentageGreen);
    dezibot.labyrinthMovement.controlMotors(isFirstGreater);
}