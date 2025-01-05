#ifndef LABYRINTH_SOLVER_H
#define LABYRINTH_SOLVER_H

#include "crossingDetection/CrossingModelT.h"
#include "crossingDetection/CrossingModelXT.h"
#include "labyrinthMapping/Graph.h"
#include "movement/LabyrinthConfig.h"
#include "movement/LabyrinthMovement.h"


enum class Marker {
    Start,
    Finish,
    Crossing,
    White,
    Path
};

class LabyrinthSolver {
private:  
    CrossingType predictCrossing(PredictionData); 
    PredictionData getSensorData();    
    bool startExploring();
    bool startSolving();

public:
    CrossingPredictorT crossingModelT;
    CrossingPredictorXT crossingModelXT;
    Graph labyrinthMap;
    LabyrinthMovement movement;
    LabyrinthConfig config;

    LabyrinthSolver();
    void begin();
    void start();

};

#endif