#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP
#include "httplib.h"

struct IController {
    virtual void registerRoutes(httplib::Server& server) = 0;
    virtual ~IController() = default;
};

#endif // ICONTROLLER_HPP
