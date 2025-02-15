#ifndef LIVEDATAPAGE_H
#define LIVEDATAPAGE_H

#include <ArduinoJson.h>
#include "PageProvider.h"
#include <map>

class LiveDataPage: public PageProvider {
private:
    WebServer* serverPointer;
    std::map<String, std::function<void(JsonObject&)>> sensorValueFunctions;
public:
    LiveDataPage(WebServer* server);
    void handler() override;
    void getEnabledSensorValues();
};

#endif //LIVEDATAPAGE_H
