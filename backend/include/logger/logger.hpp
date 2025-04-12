// logger.hpp

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>
#include <chrono>


class Logger {
public:
    Logger(const std::string& logFilePath = "logfile.txt");

    void logStartCrawl(const std::string& basePath);
    void logEndCrawl(std::chrono::duration<double> totalTime);
    void logIndexedFiles(const int indexedFilesNumber, const int totalNumberOfd);
    void logIgnoredFiles(const int ignoredFilesNumber);
    void logTotalIndexationTime(std::chrono::duration<double> totalTime);
    void logMessage(const std::string& message);
    void logSearchPerformanceTextContent(const std::string& searchQuery, double duration);
    void logSearchPerformanceFileName(const std::string& searchQuery, double duration);
    void logSearchPerformanceQuery(const std::string& searchQuery, double duration);
    void logUserSearchTextContent(const std::string& searchQuery, int resultsFound);
    void logUserSearchQuery(const std::string& searchQuery, int resultsFound);
    void logUserSearchFileName(const std::string& searchQuery, int resultsFound);
    void logFileWithNoData(const std::string& fileName);
    void logFileSizeDistribution(double avgSize, double maxSize, double minSize);


private:
    std::string logFilePath;
    void writeLog(const std::string& message);
};

extern Logger logger;

#endif // LOGGER_HPP