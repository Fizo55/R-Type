#include "Server.hpp"
#include "BinaryDeserializer.hpp"

Server::Server(std::shared_ptr<INetwork> network, std::shared_ptr<IGameLogic> gameLogic, std::shared_ptr<ILogger> logger, const std::string& ipAddress, uint16_t port)
    : network_(network), gameLogic_(gameLogic), logger_(logger), ipAddress_(ipAddress), port_(port)
{
    network_->onReceive = [this](const std::vector<uint8_t>& data, const std::string& address, uint16_t port) {
        onReceive(data, address, port);
    };
    network_->onClientConnected = [this](const std::string& address, uint16_t port) {
        onClientConnected(address, port);
    };
    network_->onClientDisconnected = [this](const std::string& address, uint16_t port) {
        onClientDisconnected(address, port);
    };
}

void Server::start()
{
    gameLogic_->initialize();
    network_->start();
    logger_->logInfo("Server is running on " + ipAddress_ + ":" + std::to_string(port_) + ". Type 'exit' to stop the server.\n");
}

void Server::stop()
{
    network_->stop();
    logger_->logInfo("Server stopped");
}

void Server::onReceive(const std::vector<uint8_t>& data, const std::string& address, uint16_t port)
{
    try {
        BinaryDeserializer deserializer(data);
        uint32_t messageType = deserializer.readUInt32();

        auto message = MessageFactory::createMessage(messageType);
        if (message) {
            message->deserialize(data);
            gameLogic_->handleMessage(message, address, port);
        } else {
            logger_->logWarning("Unknown message type received");
        }
    } catch (const std::exception& e) {
        logger_->logError(std::string("Error processing message: ") + e.what());
    }
}

void Server::onClientConnected(const std::string& address, uint16_t port) {
    logger_->logInfo("Client connected: " + address + ":" + std::to_string(port));
    // TODO : Handle connection (create new player)
}

void Server::onClientDisconnected(const std::string& address, uint16_t port) {
    logger_->logInfo("Client disconnected: " + address + ":" + std::to_string(port));
    // TODO : Handle disconnect (end game)
}
