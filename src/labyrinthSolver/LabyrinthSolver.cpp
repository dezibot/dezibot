#include "LabyrinthSolver.h"
#include "../Dezibot.h"

extern Dezibot dezibot;

LabyrinthSolver::LabyrinthSolver() {
    // TODO: Initialize class members here
}

void LabyrinthSolver::begin() {
    // TODO: alle begins etc aufrufen und schauen das es die instancen gibt evtl gar nicht und alles in die start
}

void LabyrinthSolver::start() {
    // TODO: initialisiert alles also klassen werdne gesetzt usw 
    // configer funktion wird aufgerufen
    // solve funktion wird aufgerufen
    //
}

bool LabyrinthSolver::startExploring() {
    bool foundGoal = false;
    while (!foundGoal)
    {
        Marker marker = moveUntilMarker(); // methode muss erstellt werden
        if (marker == Marker::White){
            labyrinthMap.addCrossing(CrossingType::DEAD_END);
            movement.deadEndRotation();

        }else if (marker == Marker::Finish){
            foundGoal = true;
        }else {
            PredictionData sensorData = getSensorData();
            CrossingType crossing = getCrossing(sensorData);

            Direction direction = labyrinthMap.addCrossing(crossing);

            switch (direction){
                case Direction::Left :
                    movement.moveLeft();
                case Direction::Right :
                    movement.moveRight();
                case Direction::Straight :
                    movement.moveStraight();
            }
        }
    }

    labyrinthMap.setGoalNode();    
}

CrossingType LabyrinthSolver::predictCrossing(PredictionData data) {
    PredictionData sensorData = getSensorData();
    
    CrossingType xtPrediction = crossingModelXT.predictCrossingXT(sensorData);

    if(xtPrediction == CrossingType::CROSSING_X){
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