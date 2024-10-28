#ifndef BINARYDESERIALIZER_HPP
#define BINARYDESERIALIZER_HPP

#include "IDeserializer.hpp"
#include <vector>
#include <cstdint>

class BinaryDeserializer : public IDeserializer {
public:
    BinaryDeserializer(const std::vector<uint8_t>& buffer);

    uint16_t readUInt16();
    uint32_t readUInt32();
    uint8_t readUInt8();
    float readFloat();
    std::string readString();
    std::vector<uint8_t> readBytes(size_t length);
    void BinaryDeserializer::readBytes(uint8_t *destination, size_t length);

    void readHeader(uint16_t& length, uint16_t& sequenceId, uint8_t& messageType, uint8_t& flags);
private:
    const std::vector<uint8_t>& buffer_;
    size_t offset_;
};

#endif
