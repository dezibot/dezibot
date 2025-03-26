/**
 * @file PageProvider.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the PageProvider class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "PageProvider.h"
#include <Dezibot.h>
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