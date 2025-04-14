#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <chrono>
#include "ilogger.hpp"

class Logger {
public:
    explicit Logger(ILogger* strategy);
    void setStrategy(ILogger* strategy);

    void logStartCrawl(const std::string& basePath);
    void logEndCrawl(std::chrono::duration<double> duration);
    void logIndexedFiles(int indexedFilesNumber, int totalFiles);
    void logIgnoredFiles(int ignoredFilesNumber);
    void logTotalIndexationTime(std::chrono::duration<double> totalTime);
    void logFileWithNoData(const std::string& fileName);
    void logFileSizeDistribution(double avgSize, double maxSize, double minSize);
    void logUserSearchFileName(const std::string& searchQuery, int resultsFound);
    void logUserSearchTextContent(const std::string& searchQuery, int resultsFound);
    void logSearchPerformanceFileName(const std::string& searchQuery, double duration);
    void logSearchPerformanceTextContent(const std::string& searchQuery, double duration);
    void logUserSearchQuery(const std::string& searchQuery, int resultsFound);
    void logSearchPerformanceQuery(const std::string& searchQuery, double duration);
    void logMessage(const std::string& message);

private:
    ILogger* loggerStrategy;
    void writeLog(const std::string& message);
};

#endif // LOGGER_HPP
