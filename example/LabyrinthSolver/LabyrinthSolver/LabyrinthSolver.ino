#include <Dezibot.h>
#include "LabyrinthConfig.h"
#include "LabyrinthMovement.h"
#include "Graph.h"
#include "CrossingModelT.h"
#include "CrossingModelXT.h"

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

std::array<Marker, 2> markers2 = {
Marker::Crossing,
Marker::Finish
};

int i = 0;

int markerFOund = 0;

Dezibot dezibot = Dezibot();

void setup() {
    Serial.begin(115200);
    dezibot.begin();
    dezibot.multiColorLight.setLed(BOTTOM, 88, 100, 58);

    Serial.println("start");
    delay(4000);
    // config.runSetUp();    
    // Serial.println("setUpDone");


    movement.setColorMode(RED_LEFT);
    // movement.calibrateWhite();

    crossingModelXT.initialize();    
    Serial.println("crossingModelXT.initialize();");
    crossingModelT.initialize();
    Serial.println("crossingModelT.initialize();");


    
}

void loop() {  

    // delay(2000);
    // PredictionData data = config.getSensorData();
    // Marker mark = config.getMarkerFromPrediction(data);

    // delay(1000);

    if (markerFOund < 3){
    double percentageRed, percentageGreen, percentageBlue;
    movement.getColorPercentages(percentageRed, percentageGreen, percentageBlue);     
    
      PredictionData data = config.getSensorData();
      Marker mark = config.getMarkerFromPrediction(data);

      switch (mark) {
        case Marker::Finish:
            Serial.println("Finish");
            markerFOund++;
            delay(200);
            break;
        case Marker::White:
            Serial.println("White");            
            markerFOund++;
            delay(200);
            break;
        case Marker::Crossing:
            Serial.println("Crossing");           
            markerFOund++;
            delay(200);
            break;
        case Marker::Path:
            Serial.println("Path");   
            break;
        }
    

      bool isFirstGreater = movement.compareColors(percentageRed, percentageGreen);
      movement.controlMotors(isFirstGreater);
    }else {
      movement.stopMotors();
    }


//   if (i<4){
//     Serial.print("Iteration ");
//     Serial.println(i);
    
//     // Marker marker = moveUntilMarker(); // TODO methode muss erstellt werden
//     delay(5000);
//     // Serial.println("delay ended");
//     Marker marker = markers[i];
//     // Serial.println("marker set");
    
//     if (marker == Marker::White){
//         Serial.println("marker is white");
//         labyrinthMap.addCrossing(CrossingType::DEAD_END);
//         // Serial.println("crossing has been added");
//         // movement.deadEndRotation();

//     }else if (marker == Marker::Finish){
//         foundGoal = true;
//         // Serial.println("goal found");
//     }else {
//         // Serial.println("else path");
//         PredictionData sensorData = getSensorData();
//         // Serial.println("getSensorData");
//         CrossingType crossing = predictCrossing(sensorData);
//         // Serial.println("predictCrossing");

//         DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);
//         // Serial.println("addCrossing");

//         switch (direction){
//             case DirectionLabyrinth::Left :
//                 Serial.println("-------------------Left");
//                 movement.moveLeft();
//                 break;
//             case DirectionLabyrinth::Right :
//                 Serial.println("--------------------Right");
//                 movement.moveRight();
//                 break;
//             case DirectionLabyrinth::Straight :
//                 Serial.println("--------------------Straight");
//                 movement.moveStraight();
//                 break;
        
//         }
//     }

//     if(foundGoal == true){
//         // Serial.println("for loop ended");
//         labyrinthMap.setGoalNode(); 
//         Serial.println("Ziel gefunden##########################;");
//         delay(5000); 
//         // Serial.println("delay Ended");
        
//         dezibot.display.clear();
//     }
//     i++;
//   } else if (i<6){
//      foundGoal = false;
   
//     // Marker marker = moveUntilMarker(); // TODO methode muss erstellt werden
//     delay(10000);
//     Marker marker = markers2[i];
//     if (marker == Marker::White){
//         labyrinthMap.addCrossing(CrossingType::DEAD_END);
//         movement.deadEndRotation();

//     }else if (marker == Marker::Finish){
//         foundGoal = true;
//     }else {
//         PredictionData sensorData = getSensorData();
//         CrossingType crossing = predictCrossing(sensorData);

//         DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);

//         switch (direction){
//             case DirectionLabyrinth::Left :
//                 Serial.println("-------------------Left");
//                 movement.moveLeft();
//                 break;
//             case DirectionLabyrinth::Right :
//                 Serial.println("--------------------Right");
//                 movement.moveRight();
//                 break;
//             case DirectionLabyrinth::Straight :
//                 Serial.println("--------------------Straight");
//                 movement.moveStraight();
//                 break;
//         }
//     }
    
//     if(foundGoal == true){
//         // Serial.println("for loop ended");
//         labyrinthMap.setGoalNode(); 
//         Serial.println("Ziel gefunden##########################");
//         delay(5000); 
//     }
//     i++;
//   }else {Serial.println("i bigger than 6");}
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

PredictionData getSensorData() {
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