#include "PageProvider.h"

#include <Dezibot.h>
#include <logger/Logger.h>

#include "SPIFFS.h"

String PageProvider::readHtmlFromFile(const char* filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        return {"Could not read file"};
    }
    // if (!file /*|| file.isDirectory()*/) {
    //     Logger::getInstance().logError(std::string("Could not read file ") + filename);
    //     return "Could not read file " + String(filename);
    // }

    const size_t fileSize = file.size();
    String html;
    if (!html.reserve(fileSize)) {
        Logger::getInstance().logError(std::string("Could not reserve for file ") + filename);
    }; // Pre-allocate memory to improve efficiency

    constexpr size_t bufferSize = 1024;
    char buffer[bufferSize];

    while (file.available()) {
        size_t bytesRead = file.readBytes(buffer, bufferSize);
        html.concat(buffer, bytesRead);
    }

    file.close();

    // Verify the read length matches expected size
    if (html.length() != fileSize) {
        Logger::getInstance().logError(
            std::string("File read incomplete. Expected: ")
            + std::to_string(fileSize)
            + std::string(", Actual: ")
            + std::to_string(html.length())
            + std::string(" for file ")
            + filename
        );
    }

    return html;
}