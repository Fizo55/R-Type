#ifndef IDESERIALIZER_HPP
#define IDESERIALIZER_HPP

#include <cstdint>
#include <string>

class IDeserializer {
public:
    virtual ~IDeserializer() = default;

    virtual int32_t readInt32() = 0;
    virtual uint32_t readUInt32() = 0;
    virtual float readFloat() = 0;
    virtual std::string readString() = 0;
};

#endif
