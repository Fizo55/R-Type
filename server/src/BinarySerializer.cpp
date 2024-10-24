#include "BinarySerializer.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>

BinarySerializer::BinarySerializer(std::vector<uint8_t>& buffer)
    : buffer_(buffer)
{
}

uint32_t hostToBigEndian32(uint32_t hostValue)
{
    uint8_t bytes[4];
    bytes[0] = static_cast<uint8_t>(hostValue >> 24);
    bytes[1] = static_cast<uint8_t>(hostValue >> 16);
    bytes[2] = static_cast<uint8_t>(hostValue >> 8);
    bytes[3] = static_cast<uint8_t>(hostValue);
    uint32_t bigEndianValue;
    std::memcpy(&bigEndianValue, bytes, 4);

    return bigEndianValue;
}

void BinarySerializer::writeInt32(int32_t value)
{
    uint32_t networkValue = hostToBigEndian32(static_cast<uint32_t>(value));
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeUInt32(uint32_t value)
{
    uint32_t networkValue = hostToBigEndian32(value);
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeFloat(float value)
{
    uint32_t networkValue;
    std::memcpy(&networkValue, &value, 4);
    networkValue = hostToBigEndian32(networkValue);
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeString(const std::string& value)
{
    writeUInt32(static_cast<uint32_t>(value.size()));
    buffer_.insert(buffer_.end(), value.begin(), value.end());
}
