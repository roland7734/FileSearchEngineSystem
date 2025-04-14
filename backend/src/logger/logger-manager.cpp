
#include "logger/logger-manager.hpp"
#include "logger/text-logger-strategy.hpp"
#include "logger/html-logger-strategy.hpp"

LoggerManager::LoggerManager() {
    logger = std::make_unique<Logger>(new HtmlLoggerStrategy("logfile.html"));
}

Logger& LoggerManager::instance() {
    static LoggerManager instance;
    return *(instance.logger);
}

void LoggerManager::setStrategy(std::unique_ptr<ILogger> strategy) {
    logger->setStrategy(strategy.release());
}
