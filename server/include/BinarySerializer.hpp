#ifndef BINARYSERIALIZER_HPP
#define BINARYSERIALIZER_HPP

#include "ISerializer.hpp"
#include <vector>
#include <cstdint>

class BinarySerializer : public ISerializer {
public:
    BinarySerializer(std::vector<uint8_t>& buffer);

    void writeUInt16(uint16_t value);
    void writeUInt32(uint32_t value);
    void writeUInt8(uint8_t value);
    void writeFloat(float value);
    void writeString(const std::string& value);
    void writeBytes(const std::vector<uint8_t>& data);
    void writeBytes(const uint8_t *data, size_t length);

    void writeHeader(uint16_t length, uint16_t sequenceId, uint8_t messageType, uint8_t flags);
private:
    std::vector<uint8_t>& buffer_;
};

#endif
