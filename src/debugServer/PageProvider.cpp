#include "PageProvider.h"
#include "SPIFFS.h"

String PageProvider::readHtmlFromFile(const char* filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        return {"Could not read file"};
    }
    String html = file.readString();
    file.close();
    return html;
};