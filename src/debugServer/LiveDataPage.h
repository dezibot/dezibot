#ifndef LIVEDATAPAGE_H
#define LIVEDATAPAGE_H

#include "PageProvider.h"
#include "SettingsPage.h"

class LiveDataPage: public PageProvider {
private:
    WebServer* serverPointer;
public:
    LiveDataPage(WebServer* server);
    void handler() override;
};



#endif //LIVEDATAPAGE_H
