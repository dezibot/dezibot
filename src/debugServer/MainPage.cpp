/**
 * @file MainPage.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the MainPage class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "MainPage.h"
#include "SPIFFS.h"

MainPage::MainPage(WebServer* server):serverPointer(server) {}

void MainPage::handler() {
    String htmlContent = readHtmlFromFile("/MainPage.html");
    serverPointer->send(200, "text/html", htmlContent);
};
