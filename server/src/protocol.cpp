#include "protocol.hpp"
#include <cstring>

void Protocol::serializeHeader(char *buffer, const MessageHeader& header) {
    std::memcpy(buffer, &header, sizeof(MessageHeader));
}

void Protocol::deserializeHeader(const char *buffer, MessageHeader& header) {
    std::memcpy(&header, buffer, sizeof(MessageHeader));
}

void Protocol::serializePlayerMove(char *buffer, const PlayerMove& move) {
    std::memcpy(buffer, &move, sizeof(PlayerMove));
}

void Protocol::deserializePlayerMove(const char *buffer, PlayerMove& move) {
    std::memcpy(&move, buffer, sizeof(PlayerMove));
}

void Protocol::serializeFireMissile(char *buffer, const FireMissile& missile) {
    std::memcpy(buffer, &missile, sizeof(FireMissile));
}

void Protocol::deserializeFireMissile(const char *buffer, FireMissile& missile) {
    std::memcpy(&missile, buffer, sizeof(FireMissile));
}
