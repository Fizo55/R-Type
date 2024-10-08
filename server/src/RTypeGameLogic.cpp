// RTypeGameLogic.cpp

#include "RTypeGameLogic.hpp"
#include "PlayerInputMessage.hpp"
#include "GameStateUpdateMessage.hpp"
#include "BinarySerializer.hpp"

RTypeGameLogic::RTypeGameLogic(std::shared_ptr<INetwork> network, std::shared_ptr<ILogger> logger)
    : network_(network), logger_(logger) {
}

void RTypeGameLogic::initialize() {
    // Initialization of players, for now nothing
}

void RTypeGameLogic::update(float deltaTime) {
    auto gameStateMessage = std::make_shared<GameStateUpdateMessage>();

    for (const auto& entity : entities_) {
        EntityState state;
        state.entityId = entity.id;
        state.positionX = entity.position.x;
        state.positionY = entity.position.y;
        state.velocityX = entity.velocity.x;
        state.velocityY = entity.velocity.y;
        gameStateMessage->entityStates.push_back(state);
    }

    std::vector<uint8_t> outData;
    gameStateMessage->serialize(outData);

    for (const auto& [address, player] : players_) {
        std::string ip;
        uint16_t port;
        size_t separatorPos = address.find(':');
        if (separatorPos != std::string::npos) {
            ip = address.substr(0, separatorPos);
            port = static_cast<uint16_t>(std::stoi(address.substr(separatorPos + 1)));
            network_->send(outData, ip, port);
        }
    }
}

void RTypeGameLogic::handleMessage(const std::shared_ptr<IMessage>& message, const std::string& address, uint16_t port) {
    switch (message->getType()) {
        case MessageType::PlayerInput:
            handlePlayerInput(message, address, port);
            break;
        default:
            logger_->logWarning("Received unknown message type");
            break;
    }
}

void RTypeGameLogic::handlePlayerInput(const std::shared_ptr<IMessage>& message, const std::string& address, uint16_t port) {
    auto inputMessage = std::dynamic_pointer_cast<PlayerInputMessage>(message);
    if (!inputMessage) {
        logger_->logError("Failed to cast IMessage to PlayerInputMessage");
        return;
    }

    std::string key = address + ":" + std::to_string(port);
    players_[key].inputFlags = inputMessage->inputFlags;

    logger_->logInfo("Processed input from " + key);
}
