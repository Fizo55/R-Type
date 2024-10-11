#include "GameStateUpdateMessage.hpp"
#include "BinarySerializer.hpp"
#include "BinaryDeserializer.hpp"

GameStateUpdateMessage::GameStateUpdateMessage()
    : MessageBase(MessageType::GameStateUpdate) {
}

void GameStateUpdateMessage::serialize(std::vector<uint8_t>& outData) const {
    BinarySerializer serializer(outData);
    serializer.writeUInt32(getType());
    serializer.writeUInt32(static_cast<uint32_t>(entityStates.size()));

    for (const auto& state : entityStates) {
        serializer.writeInt32(state.entityId);
        serializer.writeFloat(state.positionX);
        serializer.writeFloat(state.positionY);
        serializer.writeFloat(state.velocityX);
        serializer.writeFloat(state.velocityY);
    }
}

void GameStateUpdateMessage::deserialize(const std::vector<uint8_t>& inData) {
    BinaryDeserializer deserializer(inData);
    uint32_t entityCount = deserializer.readUInt32();
    entityStates.resize(entityCount);

    for (auto& state : entityStates) {
        state.entityId = deserializer.readInt32();
        state.positionX = deserializer.readFloat();
        state.positionY = deserializer.readFloat();
        state.velocityX = deserializer.readFloat();
        state.velocityY = deserializer.readFloat();
    }
}
