/**
 * @file Utility.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component is a collection of frequently used utility functions, mainly for converting enums to strings.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <Dezibot.h>
#include <logger/LogEntry.h>

class Utility {
private:
public:
    Utility();

    /**
     * @brief Converts a direction enum to a string.
     * @param direction The direction enum to convert.
     * @return std::string The string representation of the direction.
     */
    static String directionToString(Direction direction);

    /**
     * @brief Converts a photoTransistor enum to a string.
     * @param sensor The photoTransistor enum to convert.
     * @return std::string The string representation of the photoTransistor.
     */
    static String sensorToString(photoTransistors sensor);

    /**
     * @brief Converts a log level enum to a string.
     * @param level The log level enum to convert.
     * @return std::string The string representation of the log level.
     */
    static String logLevelToString(LogEntry::Level level);
};

#endif //UTILITY_H
