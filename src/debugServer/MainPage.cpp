#include "MainPage.h"

MainPage::MainPage(WebServer* server):serverPointer(server) {}

void MainPage::handler() {
    String s = "<html><body><h1>Main Page</h1></body></html>";
    serverPointer->send(200, "text/html", s);
};