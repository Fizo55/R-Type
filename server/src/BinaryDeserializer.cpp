#include "BinaryDeserializer.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>

BinaryDeserializer::BinaryDeserializer(const std::vector<uint8_t>& buffer)
    : buffer_(buffer), offset_(0) {
}

uint32_t bigEndianToHost32(uint32_t bigEndianValue) {
    uint8_t bytes[4];
    std::memcpy(bytes, &bigEndianValue, 4);
    return (static_cast<uint32_t>(bytes[0]) << 24) |
        (static_cast<uint32_t>(bytes[1]) << 16) |
        (static_cast<uint32_t>(bytes[2]) << 8) |
        (static_cast<uint32_t>(bytes[3]));
}

int32_t BinaryDeserializer::readInt32() {
    if (offset_ + 4 > buffer_.size()) {
        throw std::runtime_error("Buffer underflow in readInt32");
    }
    uint32_t networkValue;
    std::memcpy(&networkValue, &buffer_[offset_], 4);
    offset_ += 4;
    return static_cast<int32_t>(bigEndianToHost32(networkValue));
}

uint32_t BinaryDeserializer::readUInt32() {
    if (offset_ + 4 > buffer_.size()) {
        throw std::runtime_error("Buffer underflow in readUInt32");
    }
    uint32_t networkValue;
    std::memcpy(&networkValue, &buffer_[offset_], 4);
    offset_ += 4;
    return bigEndianToHost32(networkValue);
}

float BinaryDeserializer::readFloat() {
    if (offset_ + 4 > buffer_.size()) {
        throw std::runtime_error("Buffer underflow in readFloat");
    }
    uint32_t networkValue;
    std::memcpy(&networkValue, &buffer_[offset_], 4);
    networkValue = bigEndianToHost32(networkValue);
    float value;
    std::memcpy(&value, &networkValue, 4);
    offset_ += 4;
    return value;
}

std::string BinaryDeserializer::readString() {
    uint32_t length = readUInt32();
    if (offset_ + length > buffer_.size()) {
        throw std::runtime_error("Buffer underflow in readString");
    }
    std::string value(buffer_.begin() + offset_, buffer_.begin() + offset_ + length);
    offset_ += length;
    return value;
}
