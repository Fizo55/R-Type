#ifndef BINARYSERIALIZER_HPP
#define BINARYSERIALIZER_HPP

#include "ISerializer.hpp"
#include <vector>
#include <cstdint>

class BinarySerializer : public ISerializer {
public:
    BinarySerializer(std::vector<uint8_t>& buffer);

    void writeInt32(int32_t value) override;
    void writeUInt32(uint32_t value) override;
    void writeFloat(float value) override;
    void writeString(const std::string& value) override;

private:
    std::vector<uint8_t>& buffer_;
};

#endif
