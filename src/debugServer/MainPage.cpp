#include "MainPage.h"
#include "SPIFFS.h"

MainPage::MainPage(WebServer* server):serverPointer(server) {}

void MainPage::handler() {
    String htmlContent = readHtmlFromFile("/MainPage.html");
    serverPointer->send(200, "text/html", htmlContent);
};
