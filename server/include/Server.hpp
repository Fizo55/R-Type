// Server.hpp

#ifndef SERVER_HPP
#define SERVER_HPP

#include "INetwork.hpp"
#include "IGameLogic.hpp"
#include "ILogger.hpp"
#include "MessageFactory.hpp"
#include "MessageDispatcher.hpp"
#include <memory>

class Server {
public:
    Server(std::shared_ptr<INetwork> network, std::shared_ptr<IGameLogic> gameLogic, std::shared_ptr<ILogger> logger);

    void start();
    void stop();

private:
    void onReceive(const std::vector<uint8_t>& data, const std::string& address, uint16_t port);
    void onClientConnected(const std::string& address, uint16_t port);
    void onClientDisconnected(const std::string& address, uint16_t port);

    std::shared_ptr<INetwork> network_;
    std::shared_ptr<IGameLogic> gameLogic_;
    std::shared_ptr<ILogger> logger_;
};

#endif
