#include <Dezibot.h>
#include "Utility.h"

std::map<String, String> Utility::sensorFunctionNames = {
    {"cd_getAmbientLight", "ColorDetection.getAmbientLight()"},
    {"cd_getColorValueRed", "ColorDetection.getColorValue(VEML_RED)"},
    {"cd_getColorValueGreen", "ColorDetection.getColorValue(VEML_GREEN)"},
    {"cd_getColorValueBlue", "ColorDetection.getColorValue(VEML_BLUE)"},
    {"cd_getColorValueWhite", "ColorDetection.getColorValue(VEML_WHITE)"},
    {"ld_getValueIrfront", "LightDetection.getValue(IR_FRONT)"},
    {"ld_getValueIrleft", "LightDetection.getValue(IR_LEFT)"},
    {"ld_getValueIrright", "LightDetection.getValue(IR_RIGHT)"},
    {"ld_getValueIrback", "LightDetection.getValue(IR_BACK)"},
    {"ld_getValueDlbottom", "LightDetection.getValue(DL_BOTTOM)"},
    {"ld_getValueDlfront", "LightDetection.getValue(DL_FRONT)"},
    {"ld_getBrightestIr", "ightDetection.getBrightest(IR)"},
    {"ld_getBrightestDl", "LightDetection.getBrightest(DAYLIGHT)"},
    {"m_getSpeedLeft", "Motion.left.getSpeed()"},
    {"m_getSpeedRight", "Motion.right.getSpeed()"},
    {"md_getAcceleration", "Motion.detection.getAcceleration()"},
    {"md_getRotation", "Motion.detection.getRotation()"},
    {"md_getTemperature", "Motion.detection.getTemperature()"},
    {"md_getWhoAmI", "Motion.detection.getWhoAmI()"},
    {"md_getTilt", "Motion.detection.getTilt()"},
    {"md_getTiltDirection", "Motion.detection.getTiltDirection()"}
};

String Utility::directionToString(Direction direction) {
    switch (direction) {
        case Front:
            return "Front";
        case Left:
            return "Left";
        case Right:
            return "Right";
        case Back:
            return "Back";
        case Neutral:
            return "Neutral";
        case Flipped:
            return "Flipped";
        case Error:
            return "Error";
    }
    return "Error";
}

String Utility::sensorToString(photoTransistors sensor) {
    switch (sensor) {
        case IR_FRONT:
            return "IR_FRONT";
        case IR_LEFT:
            return "IR_LEFT";
        case IR_RIGHT:
            return "IR_RIGHT";
        case IR_BACK:
            return "IR_BACK";
        case DL_BOTTOM:
            return "DL_BOTTOM";
        case DL_FRONT:
            return "DL_FRONT";
    }
    return "Error";
}

String Utility::sensorToFunction(const String& sensor) {
    return sensorFunctionNames[sensor];
}