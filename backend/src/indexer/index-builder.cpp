// index-builder.cpp

#include "indexer/index-builder.hpp"
#include "database/database.hpp"
#include "indexer/file-crawler.hpp"
#include "config/config.hpp"
#include "logger/logger.hpp"
#include <iostream>
#include <pqxx/pqxx>

Indexer::Indexer(FileCrawler* crawler, InsertService* insertService)
        : crawler(crawler), insertService(insertService) {}

void Indexer::indexTextFiles() {
    auto start = std::chrono::high_resolution_clock::now();

    std::string basePath = crawler->getBasePath();

    if (std::filesystem::is_regular_file(basePath)) {
        logger.logMessage("The path is a file, not a directory: \"" + basePath + "\". Indexing stopped.");
        throw std::runtime_error("The path is a file, not a directory: \"" + basePath + "\". Indexing stopped.");

    } else if (!std::filesystem::is_directory(basePath)) {
        logger.logMessage("The provided path is neither a valid directory nor a valid file: \"" + basePath+ "\". Indexing stopped.");
        throw std::runtime_error("The provided path is neither a valid directory nor a valid file: \"" + basePath + "\". Indexing stopped.");
    }


    std::vector<File> files = crawler->getFilesRecursively();
    std::vector<File> batch;

    size_t numberOfBatches = files.size() / Config::BATCH_SIZE + 1;

    size_t totalSize = 0;
    double maxSize = 0;
    double minSize = files.empty() ? 0 : files[0].getSize();
    int indexedFiles = 0;
    int batchNumber = 0;

    for (const auto& file : files) {
        batch.push_back(file);

        totalSize += file.getSize();

        if (file.getSize() > maxSize) maxSize = file.getSize();
        if (file.getSize() < minSize) minSize = file.getSize();


        if (batch.size() == Config::BATCH_SIZE) {
            batchNumber++;
            if(!insertService->insertBatchToDatabase(batch))
                {
                    logger.logMessage("Insertion of Batch " + std::to_string(batchNumber) + "/" + std::to_string(numberOfBatches) + " has failed.");
                }
            else {
                logger.logMessage("Insertion of Batch " + std::to_string(batchNumber) + "/" + std::to_string(numberOfBatches) + " was successful.");
                indexedFiles += batch.size();
            }
            batch.clear();
        }
    }

    if (!batch.empty()) {
        batchNumber++;
        if(!insertService->insertBatchToDatabase(batch))
        {
            logger.logMessage("Insertion of Batch " + std::to_string(batchNumber) + "/" + std::to_string(numberOfBatches) + " has failed.");
        }
        else {
            logger.logMessage("Insertion of Batch " + std::to_string(batchNumber) + "/" + std::to_string(numberOfBatches) + " was successful.");
            indexedFiles += batch.size();
        }
        batch.clear();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    logger.logIndexedFiles(indexedFiles, files.size());
    logger.logTotalIndexationTime(duration);

    if (!files.empty()) {
        double avgSize = static_cast<double>(totalSize) / files.size();
        logger.logFileSizeDistribution(avgSize, maxSize, minSize);
    }

    logger.logMessage("Indexing completed for path: \"" + crawler->getBasePath()+"\".");
}
