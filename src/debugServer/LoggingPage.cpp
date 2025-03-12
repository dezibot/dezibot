#include "LoggingPage.h"

LoggingPage::LoggingPage(WebServer* server):serverPointer(server) {}

void LoggingPage::handler() {
    String s = "<html><body><h1>LoggingPage</h1></body></html>";
    serverPointer->send(200, "text/html", s);
};