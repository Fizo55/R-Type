#include "BinaryDeserializer.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdint>

BinaryDeserializer::BinaryDeserializer(const std::vector<uint8_t>& buffer)
    : buffer_(buffer), offset_(0) {
}

uint32_t BinaryDeserializer::toBigEndian(uint32_t value) {
    return ((value & 0xFF000000) >> 24) |
           ((value & 0x00FF0000) >> 8) |
           ((value & 0x0000FF00) << 8) |
           ((value & 0x000000FF) << 24);
}

uint32_t BinaryDeserializer::toHostOrder(uint32_t networkValue) {
    return toBigEndian(networkValue);
}

int32_t BinaryDeserializer::readInt32() {
    if (offset_ + 4 > buffer_.size()) {
        throw std::runtime_error("Buffer underflow in readInt32");
    }
    uint32_t networkValue;
    std::memcpy(&networkValue, &buffer_[offset_], 4);
    offset_ += 4;
    return static_cast<int32_t>(toHostOrder(networkValue));
}

uint32_t BinaryDeserializer::readUInt32() {
    if (offset_ + 4 > buffer_.size()) {
        throw std::runtime_error("Buffer underflow in readUInt32");
    }
    uint32_t networkValue;
    std::memcpy(&networkValue, &buffer_[offset_], 4);
    offset_ += 4;
    return toHostOrder(networkValue);
}

float BinaryDeserializer::readFloat() {
    if (offset_ + 4 > buffer_.size()) {
        throw std::runtime_error("Buffer underflow in readFloat");
    }
    uint32_t networkValue;
    std::memcpy(&networkValue, &buffer_[offset_], 4);
    networkValue = toHostOrder(networkValue);
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
