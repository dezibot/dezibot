/**
 * @file CrossingModelT.h
 * @author Vladislav Antonov, Max Mager
 * @brief Header file for the CrossingModelT class, that uses the model to Predict the type of T-Crossing.
 * 
 * @version 1.0.0
 * @date 2025-02-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef CrossingModelT_h
#define CrossingModelT_h

#include <Arduino.h>
#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "CommonDefinitions.h"


class CrossingPredictorT {
public:
    CrossingPredictorT();

    /**
     * @brief Initializes the TensorFlow Lite model.
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Predicts the Crossing based on Percentage of RGB.
     * @param SensorData all given Data from the sensor.
     * @return The predicted crossing as an enum.
     */
    CrossingType predictCrossingT(const PredictionData& SensorData);

private:
    tflite::ErrorReporter* error_reporter;
    const tflite::Model* model;
    tflite::MicroInterpreter* interpreter;
    TfLiteTensor* input;
    TfLiteTensor* output;

    static constexpr int kTensorArenaSize = 4096;
    uint8_t tensor_arena[kTensorArenaSize];

    static constexpr float redR_mean = 0.4121598114305729;
    static constexpr float greenR_mean = 0.3350287522948503;
    static constexpr float blueR_mean = 0.2528114362745768;
    static constexpr float white_mean = 7322.775669097432;
    static constexpr float ambient_mean = 117.2486820671236;

    static constexpr float redR_std = 0.019711992059990153;
    static constexpr float greenR_std = 0.009098673314281382;
    static constexpr float blueR_std = 0.01281710559531455;
    static constexpr float white_std = 561.5219636243661;
    static constexpr float ambient_std = 7.097175923362954;
 
};

#endif
