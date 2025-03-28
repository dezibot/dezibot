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
     * @brief Streams a desired file to the given webserver with an accordingly set MIME type. Makes sure that files are sent in their entirety.
     *
     * @param filename absolute path and file name inside /data/ that leads to the desired path
     * @param contentType MIME type of the content, for better specification of possible MIME types please refer to https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/MIME_types/Common_types
     */
    static void serveFileFromSpiffs(WebServer *server, const char *filename, const char *contentType);
};
#endif //PAGEPROVIDER_H
