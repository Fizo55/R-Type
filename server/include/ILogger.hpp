#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void logInfo(const std::string& message) = 0;
    virtual void logWarning(const std::string& message) = 0;
    virtual void logError(const std::string& message) = 0;
};

#endif
