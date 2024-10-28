#include "ConsoleLogger.hpp"
#include <iostream>

const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RED = "\033[31m";

void ConsoleLogger::logInfo(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex_);
    std::cout << GREEN << "[INFO] " << RESET << message << std::endl;
}

void ConsoleLogger::logWarning(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex_);
    std::cout << YELLOW << "[WARNING] " << RESET << message << std::endl;
}

void ConsoleLogger::logError(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex_);
    std::cerr << RED << "[ERROR] " << RESET << message << RESET << std::endl;
}
