#ifndef DEBUGSERVER_H
#define DEBUGSERVER_H
#include "MainPage.h"
#include "LoggingPage.h"
#include "LiveDataPage.h"
#include "SettingsPage.h"

class DebugServer {
private:
    WebServer server;
    MainPage* mainPage;
    LoggingPage* loggingPage;
    LiveDataPage* liveDataPage;
    SettingsPage* settingsPage;
public:
    DebugServer();
    void setup();
    void handleClient();
};

#endif //DEBUGSERVER_H
