#include "GameStateUpdateMessage.hpp"
#include "BinarySerializer.hpp"
#include "BinaryDeserializer.hpp"
#include "Endianness.hpp"
#include <stdexcept>

GameStateUpdateMessage::GameStateUpdateMessage()
    : MessageBase(0x11), sequenceId_(0), flags_(0)
{
}

void GameStateUpdateMessage::serialize(std::vector<uint8_t>& outBuffer, uint16_t sequenceId, uint8_t flags) const {
    BinarySerializer serializer(outBuffer);

    size_t headerStart = outBuffer.size();
    serializer.writeUInt16(0);
    serializer.writeUInt16(sequenceId);
    serializer.writeUInt8(type_);
    serializer.writeUInt8(flags);

    size_t payloadStart = outBuffer.size();

    if (entityStates.size() > UINT16_MAX)
        throw std::runtime_error("Entity count exceeds UINT16_MAX");
    serializer.writeUInt16(static_cast<uint16_t>(entityStates.size()));

    for (const auto& state : entityStates) {
        serializeEntityState(state, serializer);
    }

    uint16_t payloadLength = static_cast<uint16_t>(outBuffer.size() - payloadStart);
    uint16_t totalLength = payloadLength;

    uint16_t networkLength = Endianness::hostToNetworkUint16(totalLength);
    outBuffer[headerStart] = static_cast<uint8_t>((networkLength >> 8) & 0xFF);
    outBuffer[headerStart + 1] = static_cast<uint8_t>(networkLength & 0xFF);
}

void GameStateUpdateMessage::deserialize(const std::vector<uint8_t>& inData)
{
    BinaryDeserializer deserializer(inData);

    uint16_t length;
    uint8_t messageType;
    deserializer.readHeader(length, sequenceId_, messageType, flags_);

    if (messageType != type_)
        throw std::runtime_error("Incorrect message type for GameStateUpdateMessage");

    uint16_t entityCount = deserializer.readUInt16();
    entityStates.clear();
    entityStates.reserve(entityCount);

    for (uint16_t i = 0; i < entityCount; ++i) {
        EntityState state = deserializeEntityState(deserializer);
        entityStates.push_back(state);
    }
}

uint16_t GameStateUpdateMessage::getSequenceId() const
{
    return sequenceId_;
}

uint8_t GameStateUpdateMessage::getFlags() const
{
    return flags_;
}

void GameStateUpdateMessage::serializeEntityState(const EntityState& state, BinarySerializer& serializer) const
{
    serializer.writeUInt32(state.entityId);
    serializer.writeUInt8(state.entityType);
    serializer.writeFloat(state.positionX);
    serializer.writeFloat(state.positionY);
    serializer.writeFloat(state.velocityX);
    serializer.writeFloat(state.velocityY);

    if (state.additionalData.size() > UINT16_MAX)
        throw std::runtime_error("Additional data size exceeds UINT16_MAX");
    serializer.writeUInt16(static_cast<uint16_t>(state.additionalData.size()));
    serializer.writeBytes(state.additionalData);
}

EntityState GameStateUpdateMessage::deserializeEntityState(BinaryDeserializer& deserializer)
{
    EntityState state;
    state.entityId = deserializer.readUInt32();
    state.entityType = deserializer.readUInt8();
    state.positionX = deserializer.readFloat();
    state.positionY = deserializer.readFloat();
    state.velocityX = deserializer.readFloat();
    state.velocityY = deserializer.readFloat();

    uint16_t additionalDataLength = deserializer.readUInt16();
    state.additionalData = deserializer.readBytes(additionalDataLength);

    return state;
}