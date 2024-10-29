#pragma once

#include "IGameLogic.hpp"
#include "INetwork.hpp"
#include "ILogger.hpp"
#include "MessageBase.hpp"
#include "PlayerInputMessage.hpp"
#include "GameStateUpdateMessage.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <cstdint>

// TODO : ECS ?
enum class EntityType : uint8_t {
    Player = 0x01,
    Enemy = 0x02,
    Projectile = 0x03,
};

struct Vector2 {
    float x;
    float y;
};

// TODO : ECS ?
struct Entity {
    uint32_t id;
    EntityType type;
    Vector2 position;
    Vector2 velocity;
    std::vector<uint8_t> additionalData;
};

struct Player {
    uint32_t clientId;
    uint32_t entityId;
    uint8_t inputFlags;
    std::string address;
    uint16_t port;
};

class RTypeGameLogic : public IGameLogic {
public:
    RTypeGameLogic(std::shared_ptr<INetwork> network, std::shared_ptr<ILogger> logger);

    void initialize() override;
    void update(float deltaTime) override;
    void handleMessage(const std::shared_ptr<MessageBase>& message, const std::string& address, uint16_t port) override;
    void onClientConnected(const std::string& address, uint16_t port) override;
    void onClientDisconnected(const std::string& address, uint16_t port) override;

private:
    void handlePlayerInput(const std::shared_ptr<PlayerInputMessage>& message, const Player& player);
    void sendGameStateUpdate();

    uint32_t generateEntityId();
    uint32_t generateClientId();
    std::string getClientKey(const std::string& address, uint16_t port);

    std::shared_ptr<INetwork> network_;
    std::shared_ptr<ILogger> logger_;

    std::unordered_map<uint32_t, Entity> entities_;
    std::unordered_map<std::string, Player> players_;

    std::unordered_map<std::string, uint16_t> clientSequenceIds_;

    uint32_t nextEntityId_;
    uint32_t nextClientId_;

    float timeAccumulator_;
    const float updateInterval_;
};
