/**
 * @file PageProvider.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements an abstract class as foundation for the different pages of the webserver.
 * Each page has to implement the handler method to provide the html content.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include <WebServer.h>
#include <SPIFFS.h>
#include <Arduino.h>

class PageProvider
{
public:
    virtual ~PageProvider() = default;

    /**
     * @brief provides html on initial request from client
     * @return void
     */
    virtual void handler() = 0;

protected:
    /**
     * @brief Reads the content of a html file stored in spiffs file system and returns it as a string.
     * @param filename
     * @return String
     */
    virtual String readHtmlFromFile(const char* filename);

    static void serveFileFromSpiffs(WebServer *server, const char *filename, const char *contentType);
};
#endif //PAGEPROVIDER_H
