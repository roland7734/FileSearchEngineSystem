// logger.cpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "logger/logger.hpp"

Logger logger;

Logger::Logger(const std::string& logFilePath) : logFilePath(logFilePath) {
    writeLog("File Search Engine Log Started");
}

void Logger::writeLog(const std::string& message) {
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&nowTime);
    timeStr.erase(timeStr.find('\n'));

    logFile << "[" << timeStr << "] " << message << std::endl;
    logFile.close();
}

void Logger::logStartCrawl(const std::string& basePath) {
    std::ostringstream oss;
    oss << "Crawl started at the path \""<< basePath << "\"";
    writeLog(oss.str());
}

void Logger::logEndCrawl(std::chrono::duration<double> duration) {
    std::ostringstream oss;
    oss << "Crawl finished, "
        << "Total Duration: " << duration.count() << " seconds";
    writeLog(oss.str());
}

void Logger::logIndexedFiles(const int indexedFilesNumber, const int totalFiles) {
    std::ostringstream oss;
    oss << "Number of Indexed Files in this crawling: " << indexedFilesNumber << " out of " << totalFiles;
    writeLog(oss.str());
}

void Logger::logIgnoredFiles(const int ignoredFilesNumber) {
    std::ostringstream oss;
    oss << "Number of Ignored Files in this crawling: " << ignoredFilesNumber;
    writeLog(oss.str());
}

void Logger::logTotalIndexationTime(std::chrono::duration<double> totalTime) {
    std::ostringstream oss;
    oss << "Total Indexing Time: " << totalTime.count() << " seconds";
    writeLog(oss.str());
}


void Logger::logFileWithNoData(const std::string& fileName) {
    std::ostringstream oss;
    oss << "File '" << fileName << "' has no content.";
    writeLog(oss.str());
}
void Logger::logFileSizeDistribution(double avgSize, double maxSize, double minSize) {
    std::ostringstream oss;
    oss << "File size distribution: Average: " << avgSize << " bytes, Max: "
        << maxSize << " bytes, Min: " << minSize << " bytes.";
    writeLog(oss.str());
}

void Logger::logUserSearchFileName(const std::string& searchQuery, int resultsFound) {
    std::ostringstream oss;
    oss << "User searched for '" << searchQuery << "' in file names. Found " << resultsFound << " results.";
    writeLog(oss.str());
}

void Logger::logUserSearchTextContent(const std::string& searchQuery, int resultsFound) {
    std::ostringstream oss;
    oss << "User searched for '" << searchQuery << "' in text content. Found " << resultsFound << " results.";
    writeLog(oss.str());
}
void Logger::logSearchPerformanceFileName(const std::string& searchQuery, double duration) {
    std::ostringstream oss;
    oss << "Search for '" << searchQuery << "' in file names completed in " << duration << " seconds.";
    writeLog(oss.str());
}

void Logger::logSearchPerformanceTextContent(const std::string& searchQuery, double duration) {
    std::ostringstream oss;
    oss << "Search for '" << searchQuery << "' in text content completed in " << duration << " seconds.";
    writeLog(oss.str());
}

void Logger::logUserSearchQuery(const std::string& searchQuery, int resultsFound) {
    std::ostringstream oss;
    oss << "User searched for '" << searchQuery << "' in text content. Found " << resultsFound << " results.";
    writeLog(oss.str());
}
void Logger::logSearchPerformanceQuery(const std::string& searchQuery, double duration) {
    std::ostringstream oss;
    oss << "Search for '" << searchQuery << "' in file names completed in " << duration << " seconds.";
    writeLog(oss.str());
}


void Logger::logMessage(const std::string& message) {
    writeLog(message);
}


