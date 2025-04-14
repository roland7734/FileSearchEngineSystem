#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include "logger/text-logger-strategy.hpp"

TextLoggerStrategy::TextLoggerStrategy(const std::string& filePath)
        : logFilePath(filePath) {}

void TextLoggerStrategy::writeLog(const std::string& message) {
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
}
