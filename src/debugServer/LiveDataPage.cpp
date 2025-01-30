#include "LiveDataPage.h"
#include <Dezibot.h>

extern Dezibot dezibot;

LiveDataPage::LiveDataPage(WebServer* server):serverPointer(server)
{
    // Setup separate endpoint for AJAX requests
    server->on("/lightDetection.getValue", [this]() {
        String lightValueStr = String(dezibot.lightDetection.getValue(DL_FRONT));
        String jsonResponse = "{\"value\": " + lightValueStr + "}";
        serverPointer->send(200, "application/json", jsonResponse); // Send JSON response
    });
}

void LiveDataPage::handler() {
    String htmlContent = readHtmlFromFile("/LiveDataPage.html");
    serverPointer->send(200, "text/html", htmlContent);

};