#ifndef UTILITY_H
#define UTILITY_H

#include <Dezibot.h>
#include <logger/LogEntry.h>

class Utility {
private:
public:
    Utility();
    static String directionToString(Direction direction);
    static String sensorToString(photoTransistors sensor);
    static String logLevelToString(LogEntry::Level level);
};

#endif //UTILITY_H
