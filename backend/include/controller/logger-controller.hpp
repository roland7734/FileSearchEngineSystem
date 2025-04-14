#ifndef LOGGER_CONTROLLER_HPP
#define LOGGER_CONTROLLER_HPP

#include "icontroller.hpp"

class LoggerController : public IController {
public:
    void registerRoutes(httplib::Server& server) override;
};

#endif // LOGGER_CONTROLLER_HPP
