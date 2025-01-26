#include "SettingsPage.h"

SettingsPage::SettingsPage(WebServer* server):serverPointer(server) {}

void SettingsPage::handler() {
    String s = "<html><body><h1>Settings</h1></body></html>";
    serverPointer->send(200, "text/html", s);
};