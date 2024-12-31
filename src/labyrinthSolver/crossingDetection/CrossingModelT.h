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

    static constexpr float redR_mean = 0.4121600777315959;
    static constexpr float greenR_mean = 0.33503194216464915;
    static constexpr float blueR_mean = 0.252807980103755;

    static constexpr float redR_std = 0.019807951965642525;
    static constexpr float greenR_std = 0.009203467330839838;
    static constexpr float blueR_std = 0.012957511195991415;
 
};

#endif
