// search-service.hpp

#ifndef SEARCH_SERVICE_HPP
#define SEARCH_SERVICE_HPP

#include <vector>
#include <string>
#include "database/database.hpp"
#include "model/file.hpp"
#include "filters/IFilter.hpp"

class SearchService {
public:
    SearchService(Database* db);

    std::vector<File> searchFileNames(const std::string& keyword);
    std::vector<File> searchTextContentBySingleWord(const std::string& keyword);
    std::vector<File> searchTextContentByMultipleWords(const std::string& text);
    std::vector<File> searchQuery(const std::vector<std::unique_ptr<IFilter>>& filters);

private:
    Database* db;
};

#endif  // SEARCH_SERVICE_HPP
