#ifndef IMESSAGE_HPP
#define IMESSAGE_HPP

#include <vector>
#include <cstdint>

class IMessage {
public:
    virtual ~IMessage() = default;

    virtual uint32_t getType() const = 0;
    virtual void serialize(std::vector<uint8_t>& outData) const = 0;
    virtual void deserialize(const std::vector<uint8_t>& inData) = 0;
};

#endif
