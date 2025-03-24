// file-crawler.cpp

#include <filesystem>
#include "indexer/file-crawler.hpp"
#include "database/database.hpp"
#include "model/file.hpp"
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "utils/string-processor.hpp"
#include <vector>
#include <iostream>
#include <windows.h>
#include <pqxx/pqxx>

namespace fs = std::filesystem;

FileCrawler::FileCrawler(FileIgnorer* ignorer, const std::string& basePath)
        : ignorer(ignorer), basePath(basePath) {}

std::string FileCrawler::getBasePath() const {
    return basePath;
}

std::vector<File> FileCrawler::getFilesRecursively() const {
    std::vector<File> files;
    files.reserve(Config::BATCH_SIZE);
    logger.logStartCrawl(basePath);
    auto start = std::chrono::high_resolution_clock::now();
    std::error_code err;
    int ignoredFiles = 0;

    for (const auto& entry : fs::recursive_directory_iterator(basePath, fs::directory_options::skip_permission_denied, err)) {
        if (err) {
            std::cerr << "Iterator error: " << err.message() << std::endl;
            err.clear();
            continue;
        }
        if (entry.is_regular_file()) {
            std::wstring filePathWideString = entry.path().wstring();

            std::string filePath = StringProcessor::convertWideToUtf8(filePathWideString);

            if (ignorer && ignorer->shouldIgnore(filePath)) {
                ++ignoredFiles;
                continue;
            }
            try{
                size_t fileSize = entry.file_size();

                if (fileSize == 0) {
                    logger.logFileWithNoData(filePath);
                }

                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        entry.last_write_time() - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                std::time_t createdAt = std::chrono::system_clock::to_time_t(sctp);

                File file(filePath, fileSize, createdAt);
                files.push_back(file);

            }
            catch(const std::exception & e){
                logger.logMessage("Error: " + std::string(e.what()));
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    logger.logEndCrawl(duration);

    logger.logIgnoredFiles(ignoredFiles);

    return files;
}