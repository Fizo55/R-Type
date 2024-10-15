#ifndef MESSAGEDISPATCHER_HPP
#define MESSAGEDISPATCHER_HPP

#include "IMessage.hpp"
#include <functional>
#include <unordered_map>
#include <cstdint>

class MessageDispatcher {
public:
    void registerHandler(uint32_t messageType, std::function<void(const std::shared_ptr<IMessage>&, const std::string&, uint16_t)> handler);

    void dispatchMessage(const std::shared_ptr<IMessage>& message, const std::string& address, uint16_t port);

private:
    std::unordered_map<uint32_t, std::function<void(const std::shared_ptr<IMessage>&, const std::string&, uint16_t)>> handlers_;
};

#endif
