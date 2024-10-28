#include "BinaryDeserializer.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>
#include "Endianness.hpp"

BinaryDeserializer::BinaryDeserializer(const std::vector<uint8_t>& buffer)
    : buffer_(buffer), offset_(0)
{
}

void BinaryDeserializer::readHeader(uint16_t& length, uint16_t& sequenceId, uint8_t& messageType, uint8_t& flags) {
    length = readUInt16();
    sequenceId = readUInt16();
    messageType = readUInt8();
    flags = readUInt8();
}

uint16_t BinaryDeserializer::readUInt16()
{
    if (offset_ + 2 > buffer_.size())
        throw std::runtime_error("Buffer underflow in readUInt16");

    uint16_t networkValue;
    std::memcpy(&networkValue, &buffer_[offset_], 2);
    offset_ += 2;

    return Endianness::networkToHostUint16(networkValue);
}

uint32_t BinaryDeserializer::readUInt32() {
    if (offset_ + 4 > buffer_.size())
        throw std::runtime_error("Buffer underflow in readUInt32");

    uint8_t bytes[4];
    bytes[0] = buffer_[offset_];
    bytes[1] = buffer_[offset_ + 1];
    bytes[2] = buffer_[offset_ + 2];
    bytes[3] = buffer_[offset_ + 3];
    offset_ += 4;

    uint32_t networkValue = (static_cast<uint32_t>(bytes[0]) << 24) |
                            (static_cast<uint32_t>(bytes[1]) << 16) |
                            (static_cast<uint32_t>(bytes[2]) << 8) |
                            static_cast<uint32_t>(bytes[3]);

    uint32_t hostValue = Endianness::networkToHostUint32(networkValue);

    return hostValue;
}

uint8_t BinaryDeserializer::readUInt8()
{
    if (offset_ + 1 > buffer_.size())
        throw std::runtime_error("Buffer underflow in readUInt8");

    uint8_t value = buffer_[offset_];
    offset_ += 1;

    return value;
}

float BinaryDeserializer::readFloat() {
    if (offset_ + 4 > buffer_.size())
        throw std::runtime_error("Buffer underflow in readFloat");

    uint32_t hostValue = readUInt32();

    float value;
    std::memcpy(&value, &hostValue, sizeof(float));

    return value;
}

std::string BinaryDeserializer::readString()
{
    uint32_t length = readUInt32();

    if (offset_ + length > buffer_.size())
        throw std::runtime_error("Buffer underflow in readString");

    std::string value(buffer_.begin() + offset_, buffer_.begin() + offset_ + length);
    offset_ += length;

    return value;
}

std::vector<uint8_t> BinaryDeserializer::readBytes(size_t length)
{
    if (offset_ + length > buffer_.size())
        throw std::runtime_error("Buffer underflow in readBytes");

    std::vector<uint8_t> data(buffer_.begin() + offset_, buffer_.begin() + offset_ + length);
    offset_ += length;

    return data;
}

void BinaryDeserializer::readBytes(uint8_t *destination, size_t length)
{
    if (offset_ + length > buffer_.size())
        throw std::runtime_error("Buffer underflow in readBytes");

    std::memcpy(destination, &buffer_[offset_], length);
    offset_ += length;
}