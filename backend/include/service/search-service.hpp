#ifndef SEARCH_SERVICE_HPP
#define SEARCH_SERVICE_HPP

#include <vector>
#include <string>
#include "service/isearch-service.hpp"
#include "database/database.hpp"
#include "model/file.hpp"
#include "filters/ifilter.hpp"
#include "observers/iobserver.hpp"

class SearchService : public ISearchService{
public:
    SearchService(Database* db);

    std::vector<File> searchFileNames(const std::string& keyword);
    std::vector<File> searchTextContentBySingleWord(const std::string& keyword);
    std::vector<File> searchTextContentByMultipleWords(const std::string& text);
    std::vector<File> searchQuery(const std::vector<std::unique_ptr<IFilter>>& filters) override;
    void addObserver(IObserver* observer) override;


private:
    Database* db;
    std::vector<IObserver*> observers;
    void notifyObservers(const std::unordered_map<std::string, std::vector<std::string>>& filters, const std::unordered_set<int>& results);

    };

#endif  // SEARCH_SERVICE_HPP
