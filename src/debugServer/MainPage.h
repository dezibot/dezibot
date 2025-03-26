/**
 * @file MainPage.h
 * @author Tim Dietrich, Felix Herrling
 * @brief This component implements the main page for the webserver.
 * The main page provides navigation to other webpages.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "PageProvider.h"

class MainPage : public PageProvider {
private:
    WebServer* serverPointer;
public:
    explicit MainPage(WebServer* server);

    /**
     * @brief provides html on initial request from client
     * @return void
     */
    void handler() override;
    void cssHandler();
};

#endif //MAINPAGE_H
