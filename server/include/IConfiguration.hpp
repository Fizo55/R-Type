#ifndef ICONFIGURATION_HPP
#define ICONFIGURATION_HPP

#include <string>

class IConfiguration {
public:
    virtual ~IConfiguration() = default;

    virtual int getInt(const std::string& key, int defaultValue) const = 0;
    virtual std::string getString(const std::string& key, const std::string& defaultValue) const = 0;
};

#endif
