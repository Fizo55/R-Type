// GameStateUpdateMessage.hpp

#ifndef GAMESTATEUPDATEMESSAGE_HPP
#define GAMESTATEUPDATEMESSAGE_HPP

#include "MessageBase.hpp"
#include "ISerializer.hpp"
#include "IDeserializer.hpp"
#include "MessageTypes.hpp"
#include <vector>

// TODO : Remove that (ECS ?)
struct EntityState {
    int32_t entityId;
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
};

class GameStateUpdateMessage : public MessageBase {
public:
    GameStateUpdateMessage();

    void serialize(std::vector<uint8_t>& outData) const override;
    void deserialize(const std::vector<uint8_t>& inData) override;

    std::vector<EntityState> entityStates;
};

#endif
