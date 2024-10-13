#ifndef BINARYDESERIALIZER_HPP
#define BINARYDESERIALIZER_HPP

#include "IDeserializer.hpp"
#include <vector>
#include <cstdint>
#include <string>

class BinaryDeserializer : public IDeserializer {
public:
    BinaryDeserializer(const std::vector<uint8_t>& buffer);

    int32_t readInt32() override;
    uint32_t readUInt32() override;
    float readFloat() override;
    std::string readString() override;

private:
    static uint32_t toBigEndian(uint32_t value);
    static uint32_t toHostOrder(uint32_t networkValue);

    const std::vector<uint8_t>& buffer_;
    size_t offset_;
};

#endif
