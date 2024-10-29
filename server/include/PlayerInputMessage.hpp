#ifndef PLAYERINPUTMESSAGE_HPP
#define PLAYERINPUTMESSAGE_HPP

#include "MessageBase.hpp"
#include "ISerializer.hpp"
#include "IDeserializer.hpp"
#include "MessageTypes.hpp"

class PlayerInputMessage : public MessageBase {
public:
    PlayerInputMessage();

    uint8_t inputFlags;
    uint32_t timestamp;

    void serialize(std::vector<uint8_t>& outBuffer, uint16_t sequenceId, uint8_t flags) const override;
    void deserialize(const std::vector<uint8_t>& inBuffer) override;

    uint16_t getSequenceId() const;
    uint8_t getFlags() const;

private:
    uint16_t sequenceId_;
    uint8_t flags_;
};

#endif
