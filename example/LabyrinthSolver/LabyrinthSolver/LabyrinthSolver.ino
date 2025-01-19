#include <Dezibot.h>
#include "movement/LabyrinthConfig.h"
#include "movement/LabyrinthMovement.h"
#include "labyrinthMapping/Graph.h"
#include "crossingDetection/CrossingModelT.h"
#include "crossingDetection/CrossingModelXT.h"

CrossingPredictorT crossingModelT;
CrossingPredictorXT crossingModelXT;
Graph labyrinthMap;

LabyrinthConfig config;
LabyrinthMovement movement(config);

bool foundGoal = false;

std::array<Marker, 4> markers = {
Marker::Crossing,
Marker::White,
Marker::Crossing,
Marker::Finish
};
int i = 0;

Dezibot dezibot = Dezibot();

void setup() {
    Serial.begin(115200);
    dezibot.begin();
    dezibot.multiColorLight.setLed(BOTTOM, 88, 100, 58);

    Serial.println("setup was called");
    crossingModelXT.initialize();    
    Serial.println("crossingModelXT.initialize();");
    crossingModelT.initialize();
    Serial.println("crossingModelT.initialize();");


    
}

void loop() {   
  if (i<4){
    Serial.print("for start ");
    Serial.println(i);
    
    // Marker marker = moveUntilMarker(); // TODO methode muss erstellt werden
    delay(5000);
    Serial.println("delay ended");
    Marker marker = markers[i];
    Serial.println("marker set");
    
    if (marker == Marker::White){
        Serial.println("marker is white");
        labyrinthMap.addCrossing(CrossingType::DEAD_END);
        Serial.println("crossing has been added");
        // movement.deadEndRotation();

    }else if (marker == Marker::Finish){
        foundGoal = true;
        Serial.println("goal found");
    }else {
        Serial.println("else path");
        PredictionData sensorData = getSensorData();
        Serial.println("getSensorData");
        CrossingType crossing = predictCrossing(sensorData);
        Serial.println("predictCrossing");

        DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);
        Serial.println("addCrossing");

        switch (direction){
            case DirectionLabyrinth::Left :
                Serial.println("-------------------Left");
                movement.moveLeft();
                break;
            case DirectionLabyrinth::Right :
                Serial.println("--------------------Right");
                movement.moveRight();
                break;
            case DirectionLabyrinth::Straight :
                Serial.println("--------------------Straight");
                movement.moveStraight();
                break;
        
        }
    }

    Serial.println("for loop ended");
    labyrinthMap.setGoalNode(); 
    Serial.println("setGoalNode");
    delay(15000); 
    Serial.println("delay Ended");
      
    dezibot.display.clear();
    i++;
  } else {Serial.println("i bigger than 4");}
}


CrossingType predictCrossing(PredictionData data) {   
    Serial.println("predictionFunction"); 
    CrossingType xtPrediction = crossingModelXT.predictCrossingXT(data);
    Serial.println("predictCrossingXT"); 

    if(xtPrediction == CrossingType::X){
        Serial.println("if xtPrediction == X"); 
        return xtPrediction;
    }
    Serial.println("xtPrediction is not X"); 

    CrossingType tPrediction = crossingModelT.predictCrossingT(data);
    Serial.println("predictCrossingT"); 
    return tPrediction; 
}

PredictionData getSensorData() {
    Serial.println("getSensor Anfang");
    uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) ;
    Serial.println("VEML_RED");
    uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
    Serial.println("VEML_GREEN");
    uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
    Serial.println("VEML_BLUE");
    uint16_t white = dezibot.colorDetection.getColorValue(VEML_WHITE);
    Serial.println("VEML_WHITE");
    float ambient = dezibot.colorDetection.getAmbientLight();
    Serial.println("getAmbientLight");
    uint16_t cct = dezibot.colorDetection.gettCCT(0.0);
    Serial.println("gettCCT");
    uint16_t daylight = dezibot.lightDetection.getValue(DL_BOTTOM);
    Serial.println("lightDetection");

    Serial.println("Werte Aufgabenommen");
    
    PredictionData sensorData = {
        .red = red,
        .green = green,
        .blue = blue,
        .white = white,      
        .ambient = ambient,
        .CCT = cct,        
        .daylight = daylight    
    };

    Serial.println("Funktion Ende");
    return sensorData; 
}