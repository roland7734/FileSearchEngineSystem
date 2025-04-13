#ifndef SEARCH_SERVICE_CONTROLLER_HPP
#define SEARCH_SERVICE_CONTROLLER_HPP

#include "icontroller.hpp"
#include "service/search-service.hpp"


class SearchServiceController : public IController {
public:
    explicit SearchServiceController(SearchService* searchService);
    void registerRoutes(httplib::Server& server) override;

private:
    SearchService* searchService;
};

#endif // SEARCH_SERVICE_CONTROLLER_HPP
