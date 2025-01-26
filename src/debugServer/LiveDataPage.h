#ifndef LIVEDATAPAGE_H
#define LIVEDATAPAGE_H

#include "PageProvider.h"

class LiveDataPage: public PageProvider {
private:
    WebServer* serverPointer;
public:
    explicit LiveDataPage(WebServer* server);
    void handler() override;
};



#endif //LIVEDATAPAGE_H
