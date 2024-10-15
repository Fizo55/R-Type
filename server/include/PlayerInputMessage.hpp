#ifndef PLAYERINPUTMESSAGE_HPP
#define PLAYERINPUTMESSAGE_HPP

#include "MessageBase.hpp"
#include "ISerializer.hpp"
#include "IDeserializer.hpp"
#include "MessageTypes.hpp"

class PlayerInputMessage : public MessageBase {
public:
    PlayerInputMessage();

    void serialize(std::vector<uint8_t>& outData) const override;
    void deserialize(const std::vector<uint8_t>& inData) override;

    int32_t inputFlags;
};

#endif
