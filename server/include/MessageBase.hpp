#ifndef MESSAGEBASE_HPP
#define MESSAGEBASE_HPP

#include "IMessage.hpp"

class MessageBase : public IMessage {
public:
    MessageBase(uint32_t type);

    uint32_t getType() const override;

    void serialize(std::vector<uint8_t>& outData) const override = 0;
    void deserialize(const std::vector<uint8_t>& inData) override = 0;

private:
    uint32_t type_;
};

#endif
