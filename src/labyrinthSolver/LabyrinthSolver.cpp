#include "LabyrinthSolver.h"
#include "../Dezibot.h"

extern Dezibot dezibot;

LabyrinthSolver::LabyrinthSolver() 
    : crossingModelT(),       
      crossingModelXT(),      
      labyrinthMap(),          
      config(),           
      movement(config)  
{}

void LabyrinthSolver::begin() {
    // TODO: alle begins etc aufrufen und schauen das es die instancen gibt evtl gar nicht und alles in die start
}

void LabyrinthSolver::start() {
    // TODO: initialisiert alles also klassen werdne gesetzt usw 
    // configer funktion wird aufgerufen
    // solve funktion wird aufgerufen
    //
    movement.setColorMode(RED_LEFT);
    movement.calibrateWhite();
    dezibot.display.println("Los Gehts");
    startExploring();
    startSolving();
}

bool LabyrinthSolver::startExploring() {
    bool foundGoal = false;
    // while (!foundGoal)

    std::array<Marker, 4> markers = {
    Marker::Crossing,
    Marker::White,
    Marker::Crossing,
    Marker::Finish
    };

    for (size_t i = 0; i < 4; i++)     
    {
        // Marker marker = moveUntilMarker(); // TODO methode muss erstellt werden
        delay(10000);
        Marker marker = markers[i];
        
        // dezibot.display.println(String(marker));
        if (marker == Marker::White){
            labyrinthMap.addCrossing(CrossingType::DEAD_END);
            movement.deadEndRotation();

        }else if (marker == Marker::Finish){
            foundGoal = true;
        }else {
            PredictionData sensorData = getSensorData();
            CrossingType crossing = predictCrossing(sensorData);
            Serial.print("Prediction: ");
            // Serial.println(String(crossing));

            DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);

            switch (direction){
                case DirectionLabyrinth::Left :
                    movement.moveLeft();
                case DirectionLabyrinth::Right :
                    movement.moveRight();
                case DirectionLabyrinth::Straight :
                    movement.moveStraight();
            }
        }
    }

    labyrinthMap.setGoalNode(); 
    delay(15000); 
      
    dezibot.display.clear();
}


bool LabyrinthSolver::startSolving() {
     bool foundGoal = false;
    // while (!foundGoal)

    std::array<Marker, 2> markers = {
    Marker::Crossing,
    Marker::Finish
    };

    for (size_t i = 0; i < 2; i++)     
    {
        // Marker marker = moveUntilMarker(); // TODO methode muss erstellt werden
        delay(10000);
        Marker marker = markers[i];
        if (marker == Marker::White){
            labyrinthMap.addCrossing(CrossingType::DEAD_END);
            movement.deadEndRotation();

        }else if (marker == Marker::Finish){
            foundGoal = true;
        }else {
            PredictionData sensorData = getSensorData();
            CrossingType crossing = predictCrossing(sensorData);

            DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);

            switch (direction){
                case DirectionLabyrinth::Left :
                    movement.moveLeft();
                case DirectionLabyrinth::Right :
                    movement.moveRight();
                case DirectionLabyrinth::Straight :
                    movement.moveStraight();
            }
        }
    }

    labyrinthMap.setGoalNode(); 
    delay(15000);   
}

CrossingType LabyrinthSolver::predictCrossing(PredictionData data) {
    PredictionData sensorData = getSensorData();
    
    CrossingType xtPrediction = crossingModelXT.predictCrossingXT(sensorData);

    if(xtPrediction == CrossingType::X){
        return xtPrediction;
    }

    CrossingType tPrediction = crossingModelT.predictCrossingT(sensorData);
    return tPrediction; 
}

PredictionData LabyrinthSolver::getSensorData() {
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