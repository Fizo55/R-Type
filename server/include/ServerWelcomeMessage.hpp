#pragma once

#include "MessageBase.hpp"
#include <cstdint>
#include <vector>

class ServerWelcomeMessage : public MessageBase {
public:
    ServerWelcomeMessage();

    uint32_t clientId;

    void serialize(std::vector<uint8_t>& outBuffer, uint16_t sequenceId, uint8_t flags) const override;
    void deserialize(const std::vector<uint8_t>& inBuffer) override;

    uint16_t getSequenceId() const;
    uint8_t getFlags() const;

private:
    uint16_t sequenceId_;
    uint8_t flags_;
};
