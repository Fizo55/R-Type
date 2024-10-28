#ifndef GAMESTATEUPDATEMESSAGE_HPP
#define GAMESTATEUPDATEMESSAGE_HPP

#include "MessageBase.hpp"
#include "BinarySerializer.hpp"
#include "BinaryDeserializer.hpp"
#include "MessageTypes.hpp"
#include <vector>

// TODO : Remove that (ECS ?)
struct EntityState {
    int32_t entityId;
    uint8_t entityType;
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    std::vector<uint8_t> additionalData;
};

class GameStateUpdateMessage : public MessageBase {
public:
    GameStateUpdateMessage();

    void serialize(std::vector<uint8_t>& outBuffer, uint16_t sequenceId, uint8_t flags) const override;
    void deserialize(const std::vector<uint8_t>& inData) override;

    uint16_t getSequenceId() const;
    uint8_t getFlags() const;

    std::vector<EntityState> entityStates;

private:
    uint16_t sequenceId_;
    uint8_t flags_;

    void serializeEntityState(const EntityState& state, BinarySerializer& serializer) const;
    EntityState deserializeEntityState(BinaryDeserializer& deserializer);
};

#endif
