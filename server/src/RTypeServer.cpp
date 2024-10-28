#include "RTypeServer.hpp"
#include "BinaryDeserializer.hpp"
#include "PlayerInputMessage.hpp"
#include "ClientHelloMessage.hpp"
#include "AcknowledgmentMessage.hpp"
#include "RTypeGameLogic.hpp"
#include <thread>
#include <chrono>

RTypeServer::RTypeServer(std::shared_ptr<INetwork> network, std::shared_ptr<ILogger> logger)
    : network_(network), logger_(logger), isRunning_(false)
{
    gameLogic_ = std::make_unique<RTypeGameLogic>(network_, logger_);

    network_->setReceiveHandler([this](const std::vector<uint8_t>& data, const std::string& address, uint16_t port) {
        networkReceiveHandler(data, address, port);
    });

    network_->setClientConnectedHandler([this](const std::string& address, uint16_t port) {
        clientConnectedHandler(address, port);
    });

    network_->setClientDisconnectedHandler([this](const std::string& address, uint16_t port) {
        clientDisconnectedHandler(address, port);
    });
}

void RTypeServer::start()
{
    if (isRunning_)
        return;

    isRunning_ = true;

    gameLogic_->initialize();

    network_->start();

    logger_->logInfo("Server started.");

    while (isRunning_) {
        const float deltaTime = 0.016f;
        gameLogic_->update(deltaTime);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    network_->stop();
    logger_->logInfo("Server stopped.");
}

void RTypeServer::stop()
{
    isRunning_ = false;
}

void RTypeServer::networkReceiveHandler(const std::vector<uint8_t>& data, const std::string& address, uint16_t port)
{
    try {
        auto message = deserializeMessage(data);

        if (message) {
            gameLogic_->handleMessage(message, address, port);
        } else {
            logger_->logWarning("Received unknown or invalid message from " + address + ":" + std::to_string(port));
        }
    } catch (const std::exception& e) {
        logger_->logError("Error handling received data: " + std::string(e.what()));
    }
}

void RTypeServer::clientConnectedHandler(const std::string& address, uint16_t port)
{
    logger_->logInfo("Client connected: " + address + ":" + std::to_string(port));

    gameLogic_->onClientConnected(address, port);
}

void RTypeServer::clientDisconnectedHandler(const std::string& address, uint16_t port)
{
    logger_->logInfo("Client disconnected: " + address + ":" + std::to_string(port));

    gameLogic_->onClientDisconnected(address, port);
}

std::shared_ptr<MessageBase> RTypeServer::deserializeMessage(const std::vector<uint8_t>& data)
{
    BinaryDeserializer deserializer(data);

    uint16_t length, sequenceId;
    uint8_t messageType, flags;
    deserializer.readHeader(length, sequenceId, messageType, flags);

    std::shared_ptr<MessageBase> message;

    switch (messageType) {
        case 0x01:
        {
            auto clientHelloMsg = std::make_shared<ClientHelloMessage>();
            clientHelloMsg->deserialize(data);
            message = clientHelloMsg;
            break;
        }
        case 0x10:
        {
            auto playerInputMsg = std::make_shared<PlayerInputMessage>();
            playerInputMsg->deserialize(data);
            message = playerInputMsg;
            break;
        }
        case 0xFE:
        {
            auto ackMsg = std::make_shared<AcknowledgmentMessage>();
            ackMsg->deserialize(data);

            uint16_t ackedSequenceId = ackMsg->acknowledgedSequenceId;

            logger_->logInfo("Received acknoledgment message");
            break;
        }

        default:
            logger_->logWarning("Unknown message type: " + std::to_string(messageType));
            break;
    }

    return message;
}
