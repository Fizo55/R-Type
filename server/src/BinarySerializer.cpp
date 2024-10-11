#include "BinarySerializer.hpp"
#include <cstring>
#include <arpa/inet.h>

BinarySerializer::BinarySerializer(std::vector<uint8_t>& buffer)
    : buffer_(buffer) {
}

void BinarySerializer::writeInt32(int32_t value) {
    uint32_t networkValue = htonl(static_cast<uint32_t>(value));
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeUInt32(uint32_t value) {
    uint32_t networkValue = htonl(value);
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeFloat(float value) {
    uint32_t networkValue;
    std::memcpy(&networkValue, &value, 4);
    networkValue = htonl(networkValue);
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeString(const std::string& value) {
    writeUInt32(static_cast<uint32_t>(value.size()));
    buffer_.insert(buffer_.end(), value.begin(), value.end());
}
