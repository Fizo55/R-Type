#ifndef IGAMELOGIC_HPP
#define IGAMELOGIC_HPP

#include <memory>
#include <string>
#include <cstdint>
#include "IMessage.hpp"

class IGameLogic {
public:
    virtual ~IGameLogic() = default;

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void handleMessage(const std::shared_ptr<IMessage>& message, const std::string& address, uint16_t port) = 0;
};

#endif
