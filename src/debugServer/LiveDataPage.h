#ifndef LIVEDATAPAGE_H
#define LIVEDATAPAGE_H

#include <ArduinoJson.h>
#include "PageProvider.h"
#include <map>

class LiveDataPage: public PageProvider {
private:
    WebServer* serverPointer;
    std::map<std::string, std::function<void(JsonObject&)>> sensorValueFunctions;
public:
    explicit LiveDataPage(WebServer* server);
    void handler() override;
    void jsHandler();
    void getEnabledSensorValues();
};

#endif //LIVEDATAPAGE_H
