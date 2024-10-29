#include "ClientHelloMessage.hpp"
#include "BinarySerializer.hpp"
#include "BinaryDeserializer.hpp"
#include "Endianness.hpp"
#include <stdexcept>

ClientHelloMessage::ClientHelloMessage()
    : MessageBase(0x01), sequenceId_(0), flags_(0)
{
}

void ClientHelloMessage::serialize(std::vector<uint8_t>& outBuffer, uint16_t sequenceId, uint8_t flags) const {
    BinarySerializer serializer(outBuffer);

    size_t headerStart = outBuffer.size();
    serializer.writeUInt16(0);
    serializer.writeUInt16(sequenceId);
    serializer.writeUInt8(type_);
    serializer.writeUInt8(flags);

    size_t payloadStart = outBuffer.size();

    serializer.writeString(clientName);

    uint16_t payloadLength = static_cast<uint16_t>(outBuffer.size() - payloadStart);
    uint16_t totalLength = payloadLength;

    uint16_t networkLength = Endianness::hostToNetworkUint16(totalLength);
    outBuffer[headerStart] = static_cast<uint8_t>((networkLength >> 8) & 0xFF);
    outBuffer[headerStart + 1] = static_cast<uint8_t>(networkLength & 0xFF);
}

void ClientHelloMessage::deserialize(const std::vector<uint8_t>& inBuffer)
{
    BinaryDeserializer deserializer(inBuffer);

    uint16_t length;
    uint8_t messageType;
    deserializer.readHeader(length, sequenceId_, messageType, flags_);

    if (messageType != type_)
        throw std::runtime_error("Incorrect message type for ClientHelloMessage");

    clientName = deserializer.readString();
}

uint16_t ClientHelloMessage::getSequenceId() const
{
    return sequenceId_;
}

uint8_t ClientHelloMessage::getFlags() const
{
    return flags_;
}
