#ifndef RTYPEGAMELOGIC_HPP
#define RTYPEGAMELOGIC_HPP

#include "IGameLogic.hpp"
#include "INetwork.hpp"
#include "ILogger.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

class RTypeGameLogic : public IGameLogic {
public:
    RTypeGameLogic(std::shared_ptr<INetwork> network, std::shared_ptr<ILogger> logger);

    void initialize() override;
    void update(float deltaTime) override;
    void handleMessage(const std::shared_ptr<IMessage>& message, const std::string& address, uint16_t port) override;

private:
    void handlePlayerInput(const std::shared_ptr<IMessage>& message, const std::string& address, uint16_t port);

    std::shared_ptr<INetwork> network_;
    std::shared_ptr<ILogger> logger_;

    struct Player {
        int32_t inputFlags;
    };

    struct Entity {
        int id;
        struct {
            float x;
            float y;
        } position, velocity;
    };

    std::unordered_map<std::string, Player> players_;
    std::vector<Entity> entities_;
};

#endif
