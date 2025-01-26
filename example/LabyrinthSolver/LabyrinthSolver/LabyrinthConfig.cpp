#include "LabyrinthConfig.h"
#include <Dezibot.h>

extern Dezibot dezibot;

uint16_t LabyrinthConfig::getBaseSpeed() const {
    return baseSpeed;
}

uint16_t LabyrinthConfig::getMaxSpeed() const {
    return maxSpeed;
}

uint16_t LabyrinthConfig::getRotateSpeed() const {
    return rotateSpeed;
}

double LabyrinthConfig::getRedScale() const {
    return redScale;
}

double LabyrinthConfig::getGreenScale() const {
    return greenScale;
}

int LabyrinthConfig::getRotateDuration() const {
    return rotateDuration;
}

int LabyrinthConfig::getMoveStraightDuration() const {
    return moveStraightDuration;
}

double LabyrinthConfig::getWhiteTolerance() const {
    return whiteTolerance;
}

void LabyrinthConfig::setBaseSpeed(uint16_t speed) {
    baseSpeed = speed;
}

void LabyrinthConfig::setMaxSpeed(uint16_t speed) {
    maxSpeed = speed;
}

void LabyrinthConfig::setRotateSpeed(uint16_t speed) {
    rotateSpeed = speed;
}

void LabyrinthConfig::setRedScale(double scale) {
    redScale = scale;
}

void LabyrinthConfig::setGreenScale(double scale) {
    greenScale = scale;
}

void LabyrinthConfig::setRotateDuration(int duration) {
    rotateDuration = duration;
}

void LabyrinthConfig::setMoveStraightDuration(int duration) {
    moveStraightDuration = duration;
}

void LabyrinthConfig::setWhiteTolerance(double tolerance) {
    whiteTolerance = tolerance;
}

void LabyrinthConfig::runSetUp() {
    
    dezibot.display.println("Starting SetUpMode:");
    delay(2000);    
    dezibot.display.clear();    
    dezibot.display.println("SetUp Goal Color");
    setUpGoal();
    dezibot.display.println("SetUp White Color");
    setUpWhite();
    dezibot.display.println("SetUp Path Color");
    setUpLine();
    // setUpSpeed();

    printPredictionData();
    dezibot.display.println("SetUp Complete");
    delay(1000);   
    dezibot.display.clear();
}

void LabyrinthConfig::setUpGoal() {    
    dezibot.display.println("goal field");  
    delay(2000); 
    dezibot.display.println("measuring in 4..");
    delay(1000);
    dezibot.display.println("3");
    delay(1000);
    dezibot.display.println("2");
    delay(1000);
    dezibot.display.println("1");
    delay(1000);
    dezibot.display.clear();   

    dezibot.display.println("Dont Move");

    GoalFiled = getAverageSensor();

    delay(1000);  
    dezibot.display.clear();  
}

void LabyrinthConfig::setUpWhite() {
    dezibot.display.println("White");  
    delay(2000);  
    dezibot.display.println("measuring in 4..");
    delay(1000);
    dezibot.display.println("3");
    delay(1000);
    dezibot.display.println("2");
    delay(1000);
    dezibot.display.println("1");
    delay(1000);
    dezibot.display.clear();   

    dezibot.display.println("Dont Move");

    White = getAverageSensor();

    delay(1000); 
    dezibot.display.clear(); 
}

void LabyrinthConfig::setUpLine() {
    dezibot.display.println("middle of the Path"); 
    delay(2000);   
    dezibot.display.println("measuring in 4..");
    delay(1000);
    dezibot.display.println("3");
    delay(1000);
    dezibot.display.println("2");
    delay(1000);
    dezibot.display.println("1");
    delay(1000);
    dezibot.display.clear();   

    dezibot.display.println("Dont Move");

    PathMiddle = getAverageSensor();

    delay(1000); 
    dezibot.display.clear(); 

    dezibot.display.println("Green Path"); 
    delay(2000);   
    dezibot.display.println("measuring in 4..");
    delay(1000);
    dezibot.display.println("3");
    delay(1000);
    dezibot.display.println("2");
    delay(1000);
    dezibot.display.println("1");
    delay(1000);
    dezibot.display.clear();   

    dezibot.display.println("Dont Move");

    PathGreen = getAverageSensor();

    delay(1000); 
    dezibot.display.clear();

    dezibot.display.println("Red Path");  
    delay(2000);  
    dezibot.display.println("measuring in 4..");
    delay(1000);
    dezibot.display.println("3");
    delay(1000);
    dezibot.display.println("2");
    delay(1000);
    dezibot.display.println("1");
    delay(1000);
    dezibot.display.clear();   

    dezibot.display.println("Dont Move");

    PathRed = getAverageSensor();

    delay(1000); 
    dezibot.display.clear(); 
}

void LabyrinthConfig::setUpSpeed() {
    // TODO: Implement the function
}

PredictionData LabyrinthConfig::getSensorData() {
    uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) ;
    uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
    uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
    uint16_t white = dezibot.colorDetection.getColorValue(VEML_WHITE);
    float ambient = dezibot.colorDetection.getAmbientLight();
    uint16_t cct = dezibot.colorDetection.gettCCT(0.0);
    uint16_t daylight = dezibot.lightDetection.getValue(DL_BOTTOM);
    
    PredictionData sensorData = {
        .red = red,
        .green = green,
        .blue = blue,
        .white = white,      
        .ambient = ambient,
        .CCT = cct,        
        .daylight = daylight    
    };

    return sensorData; 
}

PredictionData LabyrinthConfig::getAverageSensor() {    
    delay(500);    
    dezibot.display.println("Measurement 1");
    PredictionData data1 = getSensorData();
    delay(500);
    dezibot.display.println("Measurement 2");
    PredictionData data2 = getSensorData();
    delay(500);
    dezibot.display.println("Measurement 3");
    PredictionData data3 = getSensorData();
    
    PredictionData averageData;
    averageData.red = (data1.red + data2.red + data3.red) / 3.0f;
    averageData.green = (data1.green + data2.green + data3.green) / 3.0f;
    averageData.blue = (data1.blue + data2.blue + data3.blue) / 3.0f;
    averageData.white = (data1.white + data2.white + data3.white) / 3.0f;
    averageData.ambient = (data1.ambient + data2.ambient + data3.ambient) / 3.0f;
    averageData.CCT = (data1.CCT + data2.CCT + data3.CCT) / 3.0f;
    averageData.daylight = (data1.daylight + data2.daylight + data3.daylight) / 3.0f;

    return averageData;
}

void LabyrinthConfig::printPredictionData() const {
    Serial.println("Prediction Data:");

    Serial.println("GoalFiled:");
    printSinglePredictionData(GoalFiled);

    Serial.println("White:");
    printSinglePredictionData(White);

    Serial.println("PathMiddle:");
    printSinglePredictionData(PathMiddle);

    Serial.println("PathRed:");
    printSinglePredictionData(PathRed);

    Serial.println("PathGreen:");
    printSinglePredictionData(PathGreen);
}

void LabyrinthConfig::printSinglePredictionData(const PredictionData& data) const {
    Serial.print("  Red: "); Serial.println(data.red, 4);
    Serial.print("  Green: "); Serial.println(data.green, 4);
    Serial.print("  Blue: "); Serial.println(data.blue, 4);
    Serial.print("  White: "); Serial.println(data.white, 4);
    Serial.print("  Ambient: "); Serial.println(data.ambient, 4);
    Serial.print("  CCT: "); Serial.println(data.CCT, 4);
    Serial.print("  Daylight: "); Serial.println(data.daylight, 4);
    Serial.println();
}