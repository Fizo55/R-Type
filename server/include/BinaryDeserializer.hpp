#ifndef BINARYDESERIALIZER_HPP
#define BINARYDESERIALIZER_HPP

#include "IDeserializer.hpp"
#include <vector>
#include <cstdint>

class BinaryDeserializer : public IDeserializer {
public:
    BinaryDeserializer(const std::vector<uint8_t>& buffer);

    int32_t readInt32() override;
    uint32_t readUInt32() override;
    float readFloat() override;
    std::string readString() override;

private:
    const std::vector<uint8_t>& buffer_;
    size_t offset_;
};

#endif
