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