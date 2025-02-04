#include <Dezibot.h>
#include "LabyrinthConfig.h"
#include "LabyrinthMovement.h"
#include "Graph.h"
#include "CrossingModelT.h"
#include "CrossingModelXT.h"
#include "PIDController.h"

CrossingPredictorT crossingModelT;
CrossingPredictorXT crossingModelXT;
Graph labyrinthMap;

LabyrinthConfig config;
LabyrinthMovement movement(config);

bool foundGoal = false;
Marker marker;
int i = 0;
int markerFOund = 0;
bool explorationDone = false;
int iterationSinceTurnCounter = 0;
ColorMode startColorMode = RED_LEFT;


// PIDController pid(10, 1, 0.005); 
PIDController pid(20, 1, 6); 


Dezibot dezibot = Dezibot();

void setup() {
    Serial.begin(115200);
    dezibot.begin();
    dezibot.multiColorLight.setLed(BOTTOM, 88, 100, 58);    
    // dezibot.colorDetection.beginAutoMode();

    dezibot.colorDetection.configure(ManuelConfig320);

    Serial.println("start");
    delay(4000);
    // config.runSetUp();    
    // Serial.println("setUpDone");


    movement.setColorMode(startColorMode);
    // movement.calibrateWhite();

    crossingModelXT.initialize();    
    Serial.println("crossingModelXT.initialize();");
    crossingModelT.initialize();
    Serial.println("crossingModelT.initialize();");


    
}

void loop() {  


    // PredictionData data = getSensorData(ManuelConfig80);
    // MotorStrength motors = pid.calculateMotorStrength(data.red, data.green, data.blue, RED_LEFT);
    
    // int leftSpeed = static_cast<int>(config.getBaseSpeed() * motors.leftMotor / 100.0);
    // int rightSpeed = static_cast<int>(config.getBaseSpeed() * motors.rightMotor / 100.0);

    // movement.setMotorSpeeds(leftSpeed, rightSpeed);

    // delay(2000);
    // PredictionData data = getSensorData(ManuelConfig320);
    // Marker mark = config.getMarkerFromPrediction(data);

    // delay(3000);

  if (!explorationDone){
    if (markerFOund < 1){
        moveUntilMarker();
    }else {
        makeDession();        

        if(foundGoal == true){
            // Serial.println("for loop ended");
            labyrinthMap.setGoalNode(); 
            Serial.println("Ziel gefunden##########################;");
            delay(5000); 
            // Serial.println("delay Ended");
            explorationDone = true;
        }  
    }
  } else {
        foundGoal = false;
        iterationSinceTurnCounter = 0;
        movement.setColorMode(startColorMode);
        if (markerFOund < 1){
            moveUntilMarker();
        }else {
            makeDession();     
            if(foundGoal == true){
                // Serial.println("for loop ended");
                labyrinthMap.setGoalNode(); 
                Serial.println("Ziel gefunden##########################");
                delay(5000); 
            }
    }
  }
}


CrossingType predictCrossing(PredictionData data) {   
    // Serial.println("predictionFunction"); 
    CrossingType xtPrediction = crossingModelXT.predictCrossingXT(data);
    // Serial.println("predictCrossingXT"); 

    if(xtPrediction == CrossingType::X){
        Serial.println("if xtPrediction == X"); 
        return xtPrediction;
    }
    // Serial.println("xtPrediction is not X"); 

    CrossingType tPrediction = crossingModelT.predictCrossingT(data);
    // Serial.println("predictCrossingT"); 
    return tPrediction; 
}

PredictionData getSensorData(VEML_CONFIG vemlConfig) {    
    dezibot.colorDetection.configure(vemlConfig);

    if (vemlConfig.exposureTime == MS80){  
        delay(90);
    }else if(vemlConfig.exposureTime == MS320) {
        delay(330);
    }
    


    // Serial.println("getSensor Anfang");
    uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) ;
    // Serial.println("VEML_RED");
    uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
    // Serial.println("VEML_GREEN");
    uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
    // Serial.println("VEML_BLUE");
    uint16_t white = dezibot.colorDetection.getColorValue(VEML_WHITE);
    // Serial.println("VEML_WHITE");
    float ambient = dezibot.colorDetection.getAmbientLight();
    // Serial.println("getAmbientLight");
    uint16_t cct = dezibot.colorDetection.gettCCT(0.0);
    // Serial.println("gettCCT");
    uint16_t daylight = dezibot.lightDetection.getValue(DL_BOTTOM);
    // Serial.println("lightDetection");

    // Serial.println("Werte Aufgabenommen");
    
    PredictionData sensorData = {
        .red = red,
        .green = green,
        .blue = blue,
        .white = white,      
        .ambient = ambient,
        .CCT = cct,        
        .daylight = daylight    
    };

    // Serial.println("Funktion Ende");
    return sensorData; 
}
void moveUntilMarker() {
        ColorMode colorMode = movement.getColorMode();

        PredictionData data = getSensorData(ManuelConfig320);
        MotorStrength motors = pid.calculateMotorStrength(data.red, data.green, data.blue, colorMode);
        
        int leftSpeed = static_cast<int>(config.getBaseSpeed() * motors.leftMotor / 100.0);
        int rightSpeed = static_cast<int>(config.getBaseSpeed() * motors.rightMotor / 100.0);

        movement.setMotorSpeeds(leftSpeed, rightSpeed);   
    
        marker = config.getMarkerFromPrediction(data);

        switch (marker) {
            case Marker::Finish:
                Serial.println("Finish");
                markerFOund++;
                delay(200);
                break;
            case Marker::White:
                if (iterationSinceTurnCounter == 0){
                    Serial.println("White");            
                    markerFOund++;
                    delay(200);
                    break;
                }
            case Marker::Crossing:
                if (iterationSinceTurnCounter == 0){
                    Serial.println("Crossing");           
                    markerFOund++;
                    delay(200);
                    break;
                }
            case Marker::Path:
                // Serial.println("Path");   
                break;
            }
        // bool isFirstGreater = movement.compareColors(percentageRed, percentageGreen);
        // movement.controlMotors(isFirstGreater);

        if (iterationSinceTurnCounter > 0) {
            iterationSinceTurnCounter--;
        }
}

void makeDession(){
        movement.stopMotors();        
        iterationSinceTurnCounter = 9;
        

        delay(3000);
        // Serial.println("delay ended");
        // Marker marker = markers[i];
        // Serial.println("marker set");       
        if (marker == Marker::White){
            Serial.println("marker is white");
            labyrinthMap.addCrossing(CrossingType::DEAD_END);
            // Serial.println("crossing has been added");
            movement.deadEndRotation();
            
            iterationSinceTurnCounter = 6;

        }else if (marker == Marker::Finish){
            foundGoal = true;
            // Serial.println("goal found");
        }else {
            // Serial.println("else path");
            PredictionData sensorData = getSensorData(ManuelConfig320);
            // Serial.println("getSensorData");
            CrossingType crossing = predictCrossing(sensorData);
            // Serial.println("predictCrossing");

            DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);
            // Serial.println("addCrossing");

            switch (direction){
                case DirectionLabyrinth::Left :
                    Serial.println("-------------------Left");                    
                    dezibot.display.println("Left");
                    movement.moveLeft();
                    break;
                case DirectionLabyrinth::Right :
                    Serial.println("--------------------Right");                 
                    dezibot.display.println("Right");
                    movement.moveRight();
                    break;
                case DirectionLabyrinth::Straight :
                    Serial.println("--------------------Straight");                 
                    dezibot.display.println("Straight");
                    movement.moveStraight();
                    break;
            
            }
        }
        
        markerFOund = 0; 
        pid.reset();  
}

