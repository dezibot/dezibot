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
int markerFound = 0;
bool explorationDone = false;
int iterationSinceTurnCounter = 0;
ColorMode startColorMode = RED_LEFT;
PIDController pid(20, 1, 6); 


Dezibot dezibot = Dezibot();

void setup() {
    Serial.begin(115200);
    dezibot.begin();
    dezibot.multiColorLight.setLed(BOTTOM, 88, 100, 58);

    dezibot.colorDetection.configure(ManualConfig80);

    Serial.println("start");
    delay(4000);

    movement.setColorMode(startColorMode);

    crossingModelXT.initialize();    
    Serial.println("crossingModelXT.initialize();");
    crossingModelT.initialize();
    Serial.println("crossingModelT.initialize();");
}

void loop() {  
  if (!explorationDone){
    if (markerFound < 1){
        moveUntilMarker();
    }else {
        makeDession();        

        if(foundGoal == true){
            labyrinthMap.setGoalNode(); 
            Serial.println("Ziel gefunden##########################;");
            delay(5000);
            explorationDone = true;
            foundGoal = false;
            iterationSinceTurnCounter = 0;
        }  
    }
  } else {
        movement.setColorMode(startColorMode);
        if (markerFound < 1){
            moveUntilMarker();
        }else {
            makeDession();     
            if(foundGoal == true){
                labyrinthMap.setGoalNode(); 
                Serial.println("Ziel gefunden##########################");
                dezibot.display.clear();
                dezibot.display.println("Ziel Gefunden Programm beendet");
                delay(50000); 
            }
    }
  }
}

CrossingType predictCrossing(PredictionData data) {
    CrossingType xtPrediction = crossingModelXT.predictCrossingXT(data);

    if(xtPrediction == CrossingType::X){
        Serial.println("if xtPrediction == X"); 
        return xtPrediction;
    }

    CrossingType tPrediction = crossingModelT.predictCrossingT(data);

    return tPrediction; 
}

PredictionData getSensorData(VEML_CONFIG vemlConfig) {    
    dezibot.colorDetection.configure(vemlConfig);

    if (vemlConfig.exposureTime == MS80){  
        delay(90);
    }else if(vemlConfig.exposureTime == MS320) {
        delay(330);
    }
    
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
void moveUntilMarker() {
        ColorMode colorMode = movement.getColorMode();

        PredictionData data = getSensorData(ManualConfig80);
        MotorStrength motors = pid.calculateMotorStrength(data.red, data.green, data.blue, colorMode);
        
        Serial.print(motors.leftMotor);
        Serial.print("    ");
        Serial.println(motors.rightMotor);
        int leftSpeed = static_cast<int>(config.getBaseSpeed() * motors.leftMotor / 100.0);
        int rightSpeed = static_cast<int>(config.getBaseSpeed() * motors.rightMotor / 100.0);


        movement.setMotorSpeeds(leftSpeed, rightSpeed);   
    
        marker = config.getMarkerFromPrediction(data);

        switch (marker) {
            case Marker::Finish:
                Serial.println("Finish");
                markerFound++;
                delay(200);
                break;
            case Marker::White:
                if (iterationSinceTurnCounter == 0){
                    Serial.println("White");            
                    markerFound++;
                    delay(200);
                    break;
                }
            case Marker::Crossing:
                if (iterationSinceTurnCounter == 0){
                    Serial.println("Crossing");           
                    markerFound++;
                    delay(200);
                    break;
                }
            case Marker::Path:
                break;
            }

        if (iterationSinceTurnCounter > 0) {
            iterationSinceTurnCounter--;
        }
}

void makeDession(){
        movement.stopMotors();        
        iterationSinceTurnCounter = 5*4; // * 4 wegen 80ms zu 320 ms
        

        delay(1000);  
        if (marker == Marker::White){
            Serial.println("marker is white");
            labyrinthMap.addCrossing(CrossingType::DEAD_END);
            movement.deadEndRotation();
            
            iterationSinceTurnCounter = 3*4; 

        }else if (marker == Marker::Finish){
            foundGoal = true;
        }else {
            PredictionData sensorData = getSensorData(ManualConfig320);
            CrossingType crossing = predictCrossing(sensorData);
            printCrossingType(crossing);

            DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);

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
        
        markerFound = 0; 
        pid.reset();  
}

void printCrossingType(CrossingType type) {
    String typeStr;

    switch (type) {
        case CrossingType::X:        typeStr = "X"; break;
        case CrossingType::T1:       typeStr = "T1"; break;
        case CrossingType::T2:       typeStr = "T2"; break;
        case CrossingType::T3:       typeStr = "T3"; break;
        case CrossingType::T:        typeStr = "T"; break;
        case CrossingType::DEAD_END: typeStr = "DEAD_END"; break;
        default:                     typeStr = "UNKNOWN"; break;
    }

    Serial.print("Crossing Type: ");
    Serial.println(typeStr);
    dezibot.display.print("Crossing Type: ");
    dezibot.display.println(typeStr);
}
