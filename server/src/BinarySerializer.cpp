#include "BinarySerializer.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdint>

BinarySerializer::BinarySerializer(std::vector<uint8_t>& buffer)
    : buffer_(buffer) {
}

uint32_t BinarySerializer::toBigEndian(uint32_t value) {
    return ((value & 0xFF000000) >> 24) |
           ((value & 0x00FF0000) >> 8) |
           ((value & 0x0000FF00) << 8) |
           ((value & 0x000000FF) << 24);
}

void BinarySerializer::writeInt32(int32_t value) {
    uint32_t networkValue = toBigEndian(static_cast<uint32_t>(value));
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeUInt32(uint32_t value) {
    uint32_t networkValue = toBigEndian(value);
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeFloat(float value) {
    uint32_t networkValue;
    std::memcpy(&networkValue, &value, 4);
    networkValue = toBigEndian(networkValue);
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeString(const std::string& value) {
    writeUInt32(static_cast<uint32_t>(value.size()));
    buffer_.insert(buffer_.end(), value.begin(), value.end());
}
