#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include <WebServer.h>

class PageProvider {
public:
    virtual ~PageProvider() = default;
    virtual void handler() = 0;
};

#endif //PAGEPROVIDER_H
