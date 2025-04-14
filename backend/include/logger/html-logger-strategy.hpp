#ifndef HTML_LOGGER_STRATEGY_HPP
#define HTML_LOGGER_STRATEGY_HPP

#include "ilogger.hpp"
#include <string>

class HtmlLoggerStrategy : public ILogger {
public:
    explicit HtmlLoggerStrategy(const std::string& filePath);
    ~HtmlLoggerStrategy();
    void writeLog(const std::string& message) override;

private:
    std::string logFilePath;
};

#endif // HTML_LOGGER_STRATEGY_HPP
