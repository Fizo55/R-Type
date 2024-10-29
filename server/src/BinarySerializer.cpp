#include "BinarySerializer.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>
#include "Endianness.hpp"

BinarySerializer::BinarySerializer(std::vector<uint8_t>& buffer)
    : buffer_(buffer)
{
}

void BinarySerializer::writeUInt32(uint32_t value)
{
    uint32_t networkValue = Endianness::hostToNetworkUint32(value);
    uint8_t bytes[4];
    std::memcpy(bytes, &networkValue, 4);
    buffer_.insert(buffer_.end(), bytes, bytes + 4);
}

void BinarySerializer::writeFloat(float value)
{
    static_assert(sizeof(float) == sizeof(uint32_t), "Size of float is not 32 bits");
    uint32_t asInt;
    std::memcpy(&asInt, &value, sizeof(float));
    writeUInt32(asInt);
}

void BinarySerializer::writeString(const std::string& value)
{
    writeUInt32(static_cast<uint32_t>(value.size()));
    buffer_.insert(buffer_.end(), value.begin(), value.end());
}

void BinarySerializer::writeUInt16(uint16_t value)
{
    uint16_t networkValue = Endianness::hostToNetworkUint16(value);
    uint8_t bytes[2];
    std::memcpy(bytes, &networkValue, 2);
    buffer_.insert(buffer_.end(), bytes, bytes + 2);
}

void BinarySerializer::writeUInt8(uint8_t value)
{
    buffer_.push_back(value);
}

void BinarySerializer::writeHeader(uint16_t length, uint16_t sequenceId, uint8_t messageType, uint8_t flags)
{
    writeUInt16(length);
    writeUInt16(sequenceId);
    writeUInt8(messageType);
    writeUInt8(flags);
}

void BinarySerializer::writeBytes(const uint8_t *data, size_t length)
{
    buffer_.insert(buffer_.end(), data, data + length);
}

void BinarySerializer::writeBytes(const std::vector<uint8_t>& data)
{
    buffer_.insert(buffer_.end(), data.begin(), data.end());
}