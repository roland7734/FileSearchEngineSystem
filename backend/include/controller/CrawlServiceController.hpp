#ifndef CRAWL_SERVICE_CONTROLLER_HPP
#define CRAWL_SERVICE_CONTROLLER_HPP


#include "controller/IController.hpp"
#include "service/insert-service.hpp"
#include "indexer/index-builder.hpp"


class CrawlServiceController : public IController {
public:
    explicit CrawlServiceController(InsertService* insertService);
    void registerRoutes(httplib::Server& server) override;

private:
    InsertService* insertService;
};

#endif // CRAWL_SERVICE_CONTROLLER_HPP
