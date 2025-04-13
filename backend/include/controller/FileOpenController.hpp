#ifndef FILE_OPEN_CONTROLLER_HPP
#define FILE_OPEN_CONTROLLER_HPP

#include "controller/IController.hpp"
#include "service/usage-stats-service.hpp"

class FileOpenController : public IController {
public:
    explicit FileOpenController(UsageStatsService* usageStatsService);
    void registerRoutes(httplib::Server& server) override;

private:
    UsageStatsService* usageStatsService;
};

#endif // FILE_OPEN_CONTROLLER_HPP
