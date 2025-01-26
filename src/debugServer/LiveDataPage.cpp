#include "LiveDataPage.h"

LiveDataPage::LiveDataPage(WebServer* server):serverPointer(server) {}

void LiveDataPage::handler() {
    String s = "<html><body><h1>Live Data</h1></body></html>";
    serverPointer->send(200, "text/html", s);
};