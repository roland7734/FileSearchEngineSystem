#ifndef SEARCH_SERVICE_CONTROLLER_HPP
#define SEARCH_SERVICE_CONTROLLER_HPP

#include "icontroller.hpp"
#include "service/search-service.hpp"
#include "cache/cached-search-service.hpp"
#include "service/month-statistics-service.hpp"


class SearchServiceController : public IController {
public:
    explicit SearchServiceController(CachedSearchService* searchService, MonthStatisticsService *statsService);
    void registerRoutes(httplib::Server& server) override;

private:
    CachedSearchService* searchService;
    MonthStatisticsService *statsService;
};

#endif // SEARCH_SERVICE_CONTROLLER_HPP
