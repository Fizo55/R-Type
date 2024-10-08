#ifndef CONSOLELOGGER_HPP
#define CONSOLELOGGER_HPP

#include "ILogger.hpp"
#include <mutex>

class ConsoleLogger : public ILogger {
public:
    void logInfo(const std::string& message) override;
    void logWarning(const std::string& message) override;
    void logError(const std::string& message) override;

private:
    std::mutex logMutex_;
};

#endif
