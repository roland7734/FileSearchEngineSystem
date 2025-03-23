// index-builder.cpp

#include "indexer/index-builder.hpp"
#include "database/database.hpp"
#include "indexer/file-crawler.hpp"
#include "config/config.hpp"
#include <iostream>

Indexer::Indexer(FileCrawler* crawler, InsertService* insertService)
        : crawler(crawler), insertService(insertService) {}

void Indexer::indexTextFiles() {
    std::vector<File> files = crawler->getFilesRecursively();
    std::vector<File> batch;

    for (const auto& file : files) {
        batch.push_back(file);

        if (batch.size() == Config::BATCH_SIZE) {
            insertService->insertBatchToDatabase(batch);
            batch.clear();
        }
    }

    if (!batch.empty()) {
        insertService->insertBatchToDatabase(batch);
    }

    std::cout << "Indexing completed for path: " << crawler->getBasePath() << std::endl;
}
