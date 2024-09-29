#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <cstdint>
#include <asio.hpp>

#pragma pack(push, 1)

struct MessageHeader {
    uint16_t messageID;
    uint16_t sequenceNo;
    uint16_t clientID;
    uint16_t flags;
};

struct PlayerMove {
    int16_t posX;
    int16_t posY;
    uint8_t direction;
};

struct FireMissile {
    uint8_t missileType;
    int16_t posX;
    int16_t posY;
};

struct ConnectRequest {
    char playerName[32];
};

struct ConnectResponse {
    uint16_t clientID;
};

struct ErrorResponse {
    uint16_t errorCode;
};

#pragma pack(pop)

class Protocol {
public:
    static void serializeHeader(char *buffer, const MessageHeader& header);
    static void deserializeHeader(const char *buffer, MessageHeader& header);

    static void serializePlayerMove(char *buffer, const PlayerMove& move);
    static void deserializePlayerMove(const char *buffer, PlayerMove& move);

    static void serializeFireMissile(char *buffer, const FireMissile& missile);
    static void deserializeFireMissile(const char *buffer, FireMissile& missile);
};

#endif // PROTOCOL_HPP
