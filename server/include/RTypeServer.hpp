#include <memory>
#include "INetwork.hpp"
#include "ILogger.hpp"
#include "IGameLogic.hpp"
#include "MessageBase.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <atomic>

class RTypeServer {
public:
    RTypeServer(std::shared_ptr<INetwork> network, std::shared_ptr<ILogger> logger);

    void start();
    void stop();

private:
    void networkReceiveHandler(const std::vector<uint8_t>& data, const std::string& address, uint16_t port);
    void clientConnectedHandler(const std::string& address, uint16_t port);
    void clientDisconnectedHandler(const std::string& address, uint16_t port);

    std::shared_ptr<MessageBase> deserializeMessage(const std::vector<uint8_t>& data);

    std::shared_ptr<INetwork> network_;
    std::shared_ptr<ILogger> logger_;
    std::unique_ptr<IGameLogic> gameLogic_;

    std::atomic<bool> isRunning_;
};
