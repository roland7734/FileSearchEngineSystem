#ifndef LOGGER_MANAGER_HPP
#define LOGGER_MANAGER_HPP

#include "logger.hpp"
#include "ilogger.hpp"
#include <memory>

class LoggerManager {
public:
    static Logger& instance();

    void setStrategy(std::unique_ptr<ILogger> strategy);

private:
    LoggerManager();
    LoggerManager(const LoggerManager&) = delete;
    LoggerManager& operator=(const LoggerManager&) = delete;

    std::unique_ptr<Logger> logger;
};

#endif // LOGGER_MANAGER_HPP
