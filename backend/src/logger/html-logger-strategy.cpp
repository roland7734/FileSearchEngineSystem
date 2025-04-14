#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include "logger/html-logger-strategy.hpp"

HtmlLoggerStrategy::HtmlLoggerStrategy(const std::string& filePath)
        : logFilePath(filePath) {
    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "<html><head><title>Log</title></head><body><ul>" << std::endl;
}

HtmlLoggerStrategy::~HtmlLoggerStrategy() {
    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "</ul></body></html>" << std::endl;
}

void HtmlLoggerStrategy::writeLog(const std::string& message) {
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile) {
        std::cerr << "Error: Unable to open HTML log file." << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&nowTime);
    timeStr.erase(timeStr.find('\n'));

    logFile << "<li><strong>[" << timeStr << "]</strong> " << message << "</li>" << std::endl;
}
