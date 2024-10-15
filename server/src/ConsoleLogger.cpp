#include "ConsoleLogger.hpp"
#include <iostream>

void ConsoleLogger::logInfo(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex_);
    std::cout << "[INFO] " << message << std::endl;
}

void ConsoleLogger::logWarning(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex_);
    std::cout << "[WARNING] " << message << std::endl;
}

void ConsoleLogger::logError(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex_);
    std::cerr << "[ERROR] " << message << std::endl;
}
