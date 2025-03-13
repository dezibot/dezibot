#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "PageProvider.h"

class SettingsPage : public PageProvider {
private:
    WebServer* serverPointer;
    void sendSensorData() const;
    void toggleSensorFunction();
public:
    explicit SettingsPage(WebServer* server);
    void handler() override;
};

#endif //SETTINGSPAGE_H
