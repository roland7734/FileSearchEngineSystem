#ifndef INDEX_BUILDER_HPP
#define INDEX_BUILDER_HPP

#include "database/database.hpp"
#include "file-crawler.hpp"
#include "service/insert-service.hpp"

#include <string>
#include <vector>

class Indexer {
public:
    Indexer(FileCrawler* crawler, InsertService* insertService);
    void indexTextFiles();


private:
    FileCrawler* crawler;
    InsertService* insertService;

};

#endif  // INDEX_BUILDER_HPP
