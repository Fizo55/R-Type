#ifndef ISERIALIZER_HPP
#define ISERIALIZER_HPP

#include <cstdint>
#include <string>
#include <vector>

class ISerializer {
public:
    virtual ~ISerializer() = default;

    virtual void writeUInt16(uint16_t value) = 0;
    virtual void writeUInt32(uint32_t value) = 0;
    virtual void writeUInt8(uint8_t value) = 0;
    virtual void writeFloat(float value) = 0;
    virtual void writeString(const std::string& value) = 0;
    virtual void writeBytes(const uint8_t* data, size_t length) = 0;
    virtual void writeBytes(const std::vector<uint8_t>& data) = 0;
};

#endif
