#include <Dezibot.h>
#include "Utility.h"

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
