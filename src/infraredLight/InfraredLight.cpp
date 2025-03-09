#include "InfraredLight.h"

#include <logger/Logger.h>

void InfraredLight::begin(void){
    bottom.begin();
    front.begin();

    Logger::getInstance().logDebug("Successfully initialized InfraredLight module");
}
