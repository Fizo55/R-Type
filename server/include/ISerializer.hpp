#ifndef ISERIALIZER_HPP
#define ISERIALIZER_HPP

#include <cstdint>
#include <string>

class ISerializer {
public:
    virtual ~ISerializer() = default;

    virtual void writeInt32(int32_t value) = 0;
    virtual void writeUInt32(uint32_t value) = 0;
    virtual void writeFloat(float value) = 0;
    virtual void writeString(const std::string& value) = 0;
};

#endif
