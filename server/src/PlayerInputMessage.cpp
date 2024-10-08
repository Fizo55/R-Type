#include "PlayerInputMessage.hpp"
#include "BinarySerializer.hpp"
#include "BinaryDeserializer.hpp"

PlayerInputMessage::PlayerInputMessage()
    : MessageBase(MessageType::PlayerInput), inputFlags(0) {
}

void PlayerInputMessage::serialize(std::vector<uint8_t>& outData) const {
    BinarySerializer serializer(outData);
    serializer.writeUInt32(getType());
    serializer.writeInt32(inputFlags);
}

void PlayerInputMessage::deserialize(const std::vector<uint8_t>& inData) {
    BinaryDeserializer deserializer(inData);
    inputFlags = deserializer.readInt32();
}
