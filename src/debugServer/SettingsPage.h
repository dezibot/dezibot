#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "PageProvider.h"

class SettingsPage : public PageProvider {
private:
    WebServer* serverPointer;
public:
    explicit SettingsPage(WebServer* server);
    void handler() override;
};

#endif //SETTINGSPAGE_H
