#include "RTypeGameLogic.hpp"
#include "PlayerInputMessage.hpp"
#include "GameStateUpdateMessage.hpp"
#include "BinarySerializer.hpp"
#include "ServerWelcomeMessage.hpp"

RTypeGameLogic::RTypeGameLogic(std::shared_ptr<INetwork> network, std::shared_ptr<ILogger> logger)
    : network_(network), logger_(logger), nextEntityId_(1), nextClientId_(1),
    timeAccumulator_(0.0f), updateInterval_(0.05f)
{
}

void RTypeGameLogic::initialize()
{
    logger_->logInfo("Game logic initialized.");
}

void RTypeGameLogic::update(float deltaTime)
{
    timeAccumulator_ += deltaTime;

    for (auto& [id, entity] : entities_) {
        entity.position.x += entity.velocity.x * deltaTime;
        entity.position.y += entity.velocity.y * deltaTime;
    }

    if (timeAccumulator_ >= updateInterval_) {
        sendGameStateUpdate();
        timeAccumulator_ = 0.0f;
    }
}

void RTypeGameLogic::handleMessage(const std::shared_ptr<MessageBase>& message, const std::string& address, uint16_t port)
{
    std::string clientKey = getClientKey(address, port);
    auto it = players_.find(clientKey);

    if (it == players_.end()) {
        logger_->logWarning("Received message from unknown client: " + clientKey);
        return;
    }

    Player& player = it->second;

    switch (message->getType()) {
    case 0x10:
    {
        auto inputMessage = std::dynamic_pointer_cast<PlayerInputMessage>(message);
        if (inputMessage) {
            handlePlayerInput(inputMessage, player);
        }
        else {
            logger_->logError("Failed to cast MessageBase to PlayerInputMessage");
        }
        break;
    }
    default:
        logger_->logWarning("Received unknown message type: " + std::to_string(message->getType()));
        break;
    }
}

void RTypeGameLogic::onClientConnected(const std::string& address, uint16_t port)
{
    std::string clientKey = getClientKey(address, port);

    if (players_.find(clientKey) != players_.end()) {
        logger_->logWarning("Client already connected: " + clientKey);
        return;
    }

    uint32_t clientId = generateClientId();
    uint32_t entityId = generateEntityId();

    Entity playerEntity;
    playerEntity.id = entityId;
    playerEntity.type = EntityType::Player;
    playerEntity.position = { 0.0f, 0.0f };
    playerEntity.velocity = { 0.0f, 0.0f };
    entities_[entityId] = playerEntity;

    Player player;
    player.clientId = clientId;
    player.entityId = entityId;
    player.inputFlags = 0;
    player.address = address;
    player.port = port;
    players_[clientKey] = player;

    ServerWelcomeMessage welcomeMsg;
    welcomeMsg.clientId = clientId;

    std::vector<uint8_t> outBuffer;
    uint16_t sequenceId = clientSequenceIds_[clientKey]++;
    uint8_t flags = 0x01;
    welcomeMsg.serialize(outBuffer, sequenceId, flags);

    network_->send(outBuffer, address, port);

    logger_->logInfo("Client connected: " + clientKey + ", assigned clientId: " + std::to_string(clientId));
}

void RTypeGameLogic::onClientDisconnected(const std::string& address, uint16_t port)
{
    std::string clientKey = getClientKey(address, port);

    auto it = players_.find(clientKey);
    if (it != players_.end()) {
        uint32_t entityId = it->second.entityId;
        players_.erase(it);
        entities_.erase(entityId);
        clientSequenceIds_.erase(clientKey);

        logger_->logInfo("Client disconnected: " + clientKey);
    }
    else {
        logger_->logWarning("Unknown client disconnected: " + clientKey);
    }
}

void RTypeGameLogic::handlePlayerInput(const std::shared_ptr<PlayerInputMessage>& message, const Player& player)
{
    auto entityIt = entities_.find(player.entityId);
    if (entityIt == entities_.end()) {
        logger_->logError("Entity not found for player clientId: " + std::to_string(player.clientId));
        return;
    }

    Entity& entity = entityIt->second;

    uint8_t inputFlags = message->inputFlags;

    entity.velocity.x = 0.0f;
    entity.velocity.y = 0.0f;

    const float speed = 100.0f;

    if (inputFlags & 0x01) { // Move Up
        entity.velocity.y = -speed;
    }
    if (inputFlags & 0x02) { // Move Down
        entity.velocity.y = speed;
    }
    if (inputFlags & 0x04) { // Move Left
        entity.velocity.x = -speed;
    }
    if (inputFlags & 0x08) { // Move Right
        entity.velocity.x = speed;
    }
    if (inputFlags & 0x10) { // Fire
        // TODO
    }

    logger_->logInfo("Processed input for clientId: " + std::to_string(player.clientId));
}

void RTypeGameLogic::sendGameStateUpdate()
{
    GameStateUpdateMessage gameStateMsg;

    for (const auto& [id, entity] : entities_) {
        EntityState state;
        state.entityId = entity.id;
        state.entityType = static_cast<uint8_t>(entity.type);
        state.positionX = entity.position.x;
        state.positionY = entity.position.y;
        state.velocityX = entity.velocity.x;
        state.velocityY = entity.velocity.y;
        state.additionalData = entity.additionalData;
        gameStateMsg.entityStates.push_back(state);
    }

    for (const auto& [clientKey, player] : players_) {
        std::vector<uint8_t> outBuffer;
        uint16_t sequenceId = clientSequenceIds_[clientKey]++;
        uint8_t flags = 0x01;
        gameStateMsg.serialize(outBuffer, sequenceId, flags);

        network_->send(outBuffer, player.address, player.port);
    }

    logger_->logInfo("Sent game state update to clients.");
}

uint32_t RTypeGameLogic::generateEntityId()
{
    return nextEntityId_++;
}

uint32_t RTypeGameLogic::generateClientId()
{
    return nextClientId_++;
}

std::string RTypeGameLogic::getClientKey(const std::string& address, uint16_t port)
{
    return address + ":" + std::to_string(port);
}
