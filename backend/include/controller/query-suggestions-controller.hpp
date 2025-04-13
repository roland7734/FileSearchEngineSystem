#ifndef QUERY_SUGGESTIONS_CONTROLLER_HPP
#define QUERY_SUGGESTIONS_CONTROLLER_HPP

#include "icontroller.hpp"
#include "observers/search-history.hpp"

class QuerySuggestionsController : public IController {
public:
    explicit QuerySuggestionsController(SearchHistory* searchHistory);
    void registerRoutes(httplib::Server& server) override;

private:
    SearchHistory* searchHistory;
};

#endif // QUERY_SUGGESTIONS_CONTROLLER_HPP
