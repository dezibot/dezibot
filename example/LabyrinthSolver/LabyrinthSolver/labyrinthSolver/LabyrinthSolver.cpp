// #include "LabyrinthSolver.h"
// #include "../../../src/Dezibot.h"

// extern Dezibot dezibot;

// LabyrinthSolver::LabyrinthSolver() 
//     : crossingModelT(),       
//       crossingModelXT(),      
//       labyrinthMap(),          
//       config(),           
//       movement(config)  
// {}

// void LabyrinthSolver::begin() {
//     // TODO: alle begins etc aufrufen und schauen das es die instancen gibt evtl gar nicht und alles in die start
// }

// void LabyrinthSolver::start() {    

//     // Serial.println("start was called");
//     // crossingModelXT.initialize();    
//     // Serial.println("crossingModelXT.initialize();");
//     // crossingModelT.initialize();
//     // Serial.println("crossingModelT.initialize();");

//     // PredictionData sensorData = getSensorData();
//     // Serial.println("getSensorData");
//     // CrossingType crossing = predictCrossing(sensorData);
//     // Serial.println("predictCrossing");


//     // TODO: initialisiert alles also klassen werdne gesetzt usw 
//     // configer funktion wird aufgerufen
//     // solve funktion wird aufgerufen
//     //
//     // Serial.println("First Serial MEssage");
//     // movement.setColorMode(RED_LEFT);
//     // Serial.println("setColorMode");
//     // movement.calibrateWhite();
//     // Serial.println("calibrateWhite");
//     // delay(6500);
//     // Serial.println("delay(6500);");
//     // dezibot.display.println("Los Gehts");
//     // Serial.println("dezibot.display");


//     Serial.println("startExploring start");
//     startExploring();
//     // Serial.println("startExploring ENded");
//     // Serial.println("startExploring");
//     // startSolving();
    
// }

// bool LabyrinthSolver::startExploring() {
//     Serial.println("startExploring");
//     int testI = 0;
//     // in while auf currentValuetesten
//     bool currentValue = true; 
//     bool currentValue2 = false;
//     while (currentValue)
//     {
//         Serial.print("while start ");
//         Serial.println(testI);
//         testI = testI + 1;        
//         Serial.print("testI<4 ");
//         currentValue = testI<4;
//         Serial.println(currentValue);
//         currentValue2 = testI>4; 
//         Serial.print("testI>4 ");
//         Serial.println(currentValue2);
//         delay(500);
//         if(testI>=4){
//             break;
//         }
//     }
    
//     Serial.println("While Entkommen------------");
//     // bool foundGoal = false;
//     // while (!foundGoal)

//     // std::array<Marker, 4> markers = {
//     // Marker::Crossing,
//     // Marker::White,
//     // Marker::Crossing,
//     // Marker::Finish
//     // };

//     // for (int i = 0; i < 4; i++)     
//     // {
        
//     //     Serial.print("for start ");
//     //     Serial.println(i);
        
//         // Marker marker = moveUntilMarker(); // TODO methode muss erstellt werden
//         // delay(5000);
//         // Serial.println("delay ended");
//         // Marker marker = markers[i];
//         // Serial.println("marker set");
        
//         // dezibot.display.println(String(marker));
//         // if (marker == Marker::White){
//             // Serial.println("marker is white");
//             // labyrinthMap.addCrossing(CrossingType::DEAD_END);
//             // Serial.println("crossing has been added");
//             // // movement.deadEndRotation();

//         // }else if (marker == Marker::Finish){
//             // foundGoal = true;
//             // Serial.println("goal found");
//         // }else {
//             // Serial.println("else path");
//             // PredictionData sensorData = getSensorData();
//             // Serial.println("getSensorData");
//             // CrossingType crossing = predictCrossing(sensorData);
//             // // CrossingType crossing = CrossingType::X;
//             // Serial.println("predictCrossing");

//             // DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);
//             // Serial.println("addCrossing");

//             // switch (direction){
//             //     case DirectionLabyrinth::Left :
//             //         Serial.println("-------------------Left");
//             //         // movement.moveLeft();
//             //         break;
//             //     case DirectionLabyrinth::Right :
//             //         Serial.println("--------------------Right");
//             //         // movement.moveRight();
//             //         break;
//             //     case DirectionLabyrinth::Straight :
//             //         Serial.println("--------------------Straight");
//             //         // movement.moveStraight();
//             //         break;
//             // }
//         // }
//     // }

//     // Serial.println("for loop ended");
//     // labyrinthMap.setGoalNode(); 
//     // Serial.println("setGoalNode");
//     // delay(15000); 
//     // Serial.println("delay Ended");
      
//     // dezibot.display.clear();
// }


// bool LabyrinthSolver::startSolving() {
//      bool foundGoal = false;
//     // while (!foundGoal)

//     std::array<Marker, 2> markers = {
//     Marker::Crossing,
//     Marker::Finish
//     };

//     for (size_t i = 0; i < 2; i++)     
//     {
//         // Marker marker = moveUntilMarker(); // TODO methode muss erstellt werden
//         delay(10000);
//         Marker marker = markers[i];
//         if (marker == Marker::White){
//             labyrinthMap.addCrossing(CrossingType::DEAD_END);
//             movement.deadEndRotation();

//         }else if (marker == Marker::Finish){
//             foundGoal = true;
//         }else {
//             PredictionData sensorData = getSensorData();
//             CrossingType crossing = predictCrossing(sensorData);

//             DirectionLabyrinth direction = labyrinthMap.addCrossing(crossing);

//             switch (direction){
//                 case DirectionLabyrinth::Left :
//                     movement.moveLeft();
//                 case DirectionLabyrinth::Right :
//                     movement.moveRight();
//                 case DirectionLabyrinth::Straight :
//                     movement.moveStraight();
//             }
//         }
//     }

//     labyrinthMap.setGoalNode(); 
//     delay(15000);   
// }

// CrossingType LabyrinthSolver::predictCrossing(PredictionData data) {   
//     Serial.println("predictionFunction"); 
//     CrossingType xtPrediction = crossingModelXT.predictCrossingXT(data);
//     Serial.println("predictCrossingXT"); 

//     if(xtPrediction == CrossingType::X){
//         Serial.println("if xtPrediction == X"); 
//         return xtPrediction;
//     }
//     Serial.println("xtPrediction is not X"); 

//     CrossingType tPrediction = crossingModelT.predictCrossingT(data);
//     Serial.println("predictCrossingT"); 
//     return tPrediction; 
// }

// PredictionData LabyrinthSolver::getSensorData() {
//     Serial.println("getSensor Anfang");
//     uint16_t red = dezibot.colorDetection.getColorValue(VEML_RED) ;
//     Serial.println("VEML_RED");
//     uint16_t green = dezibot.colorDetection.getColorValue(VEML_GREEN);
//     Serial.println("VEML_GREEN");
//     uint16_t blue = dezibot.colorDetection.getColorValue(VEML_BLUE);
//     Serial.println("VEML_BLUE");
//     uint16_t white = dezibot.colorDetection.getColorValue(VEML_WHITE);
//     Serial.println("VEML_WHITE");
//     float ambient = dezibot.colorDetection.getAmbientLight();
//     Serial.println("getAmbientLight");
//     uint16_t cct = dezibot.colorDetection.gettCCT(0.0);
//     Serial.println("gettCCT");
//     uint16_t daylight = dezibot.lightDetection.getValue(DL_BOTTOM);
//     Serial.println("lightDetection");

//     Serial.println("Werte Aufgabenommen");
    
//     PredictionData sensorData = {
//         .red = red,
//         .green = green,
//         .blue = blue,
//         .white = white,      
//         .ambient = ambient,
//         .CCT = cct,        
//         .daylight = daylight    
//     };

//     Serial.println("Funktion Ende");
//     return sensorData; 
// }