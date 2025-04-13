#ifndef SEARCHSERVICECONTROLLER_HPP
#define SEARCHSERVICECONTROLLER_HPP

#include "IController.hpp"
#include "service/search-service.hpp"


class SearchServiceController : public IController {
public:
    explicit SearchServiceController(SearchService* searchService);
    void registerRoutes(httplib::Server& server) override;

private:
    SearchService* searchService;
};

#endif // SEARCHSERVICECONTROLLER_HPP
