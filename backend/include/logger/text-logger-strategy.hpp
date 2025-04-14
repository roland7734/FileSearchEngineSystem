#ifndef TEXT_LOGGER_STRATEGY_HPP
#define TEXT_LOGGER_STRATEGY_HPP

#include "ilogger.hpp"
#include <string>

class TextLoggerStrategy : public ILogger {
public:
    explicit TextLoggerStrategy(const std::string& filePath);
    void writeLog(const std::string& message) override;

private:
    std::string logFilePath;
};

#endif // TEXT_LOGGER_STRATEGY_HPP
