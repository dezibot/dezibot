// SensorPredictor.cpp
#include "CrossingModelXT.h"
#include "modelXT.h"

CrossingPredictorXT::CrossingPredictorXT()
    : error_reporter(nullptr), model(nullptr), interpreter(nullptr), input(nullptr), output(nullptr) {}

bool CrossingPredictorXT::initialize() {
    
    // Set up TensorFlow Lite
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model schema version %d not supported. Supported: %d",
                             model->version(), TFLITE_SCHEMA_VERSION);
        return false;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Tensor allocation failed");
        return false;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    return true;
}

CrossingType CrossingPredictorXT::predictCrossingXT(const PredictionData& SensorData){
    float total_rgb = SensorData.red + SensorData.green + SensorData.blue;
    float red_ratio = SensorData.red / total_rgb;
    float green_ratio = SensorData.green / total_rgb;
    float blue_ratio = SensorData.blue / total_rgb;

    input->data.f[0] = (red_ratio - redR_mean) / redR_std;
    input->data.f[1] = (green_ratio - greenR_mean) / greenR_std;
    input->data.f[2] = (blue_ratio - blueR_mean) / blueR_std;
    input->data.f[3] = (SensorData.ambient - ambient_mean) / ambient_std;

    // Perform inference
    if (interpreter->Invoke() != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Inference failed");
        return CrossingType::X; 
    }

    // Interpret output
    float t_score = output->data.f[0];
    float x_score = output->data.f[1];

    return (t_score > x_score) ? CrossingType::T : CrossingType::X;
}
