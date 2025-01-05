#ifndef CrossingModelXT_h
#define CrossingModelXT_h

#include <Arduino.h>
#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "../CommonDefinitions.h"



class CrossingPredictorXT {
public:
    CrossingPredictorXT();

    /**
     * @brief Initializes the TensorFlow Lite model.
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Predicts the Crossing based on Percentage of RGB and ambient values.
     * @param SensorData all given Data from the sensor.
     * @return The predicted crossing as an enum.
     */
    CrossingType predictCrossingXT(const PredictionData& SensorData);

private:
    tflite::ErrorReporter* error_reporter;
    const tflite::Model* model;
    tflite::MicroInterpreter* interpreter;
    TfLiteTensor* input;
    TfLiteTensor* output;

    static constexpr int kTensorArenaSize = 4096;
    uint8_t tensor_arena[kTensorArenaSize];

    // Scaling parameters
    static constexpr float redR_mean = 0.40267303379988123;
    static constexpr float greenR_mean = 0.3370072760088773;
    static constexpr float blueR_mean = 0.26031969019124146;
    static constexpr float ambient_mean = 107.15621361221795;

    static constexpr float redR_std = 0.018831508508453654;
    static constexpr float greenR_std = 0.007886784461911484;
    static constexpr float blueR_std = 0.012966015626683512;
    static constexpr float ambient_std = 107.15621361221795;
};

#endif
