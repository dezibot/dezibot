
#ifndef UTILITY_H
#define UTILITY_H

class Utility {
private:
    static std::map<String, String> sensorFunctionNames;
public:
    Utility();
    static String directionToString(Direction direction);
    static String sensorToString(photoTransistors sensor);
    static String sensorToFunction(const String& sensor);
};

#endif //UTILITY_H
