#include "MainPage.h"

#include <logger/Logger.h>

MainPage::MainPage(WebServer* server):serverPointer(server) {}

void MainPage::handler() {
    Logger::getInstance().logTrace("Handling request for Main Page");
    String htmlContent = readHtmlFromFile("/MainPage.html");
    serverPointer->send(200, "text/html", htmlContent);

};

void MainPage::cssHandler() {
    Logger::getInstance().logTrace("Handling request of Main Page Style");
    String cssContent = readHtmlFromFile("/style.css");
    serverPointer->send(200, "text/css", cssContent);
}

