#ifndef IDESERIALIZER_HPP
#define IDESERIALIZER_HPP

#include <cstdint>
#include <string>
#include <vector>

class IDeserializer {
public:
    virtual ~IDeserializer() = default;

    virtual uint16_t readUInt16() = 0;
    virtual uint32_t readUInt32() = 0;
    virtual uint8_t readUInt8() = 0;
    virtual float readFloat() = 0;
    virtual std::string readString() = 0;
    virtual std::vector<uint8_t> readBytes(size_t length) = 0;
    virtual void readHeader(uint16_t& length, uint16_t& sequenceId, uint8_t& messageType, uint8_t& flags) = 0;
};

#endif
