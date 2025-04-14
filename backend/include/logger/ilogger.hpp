#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void writeLog(const std::string& message) = 0;
};

#endif // ILOGGER_HPP
