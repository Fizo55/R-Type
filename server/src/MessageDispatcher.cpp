#include "MessageDispatcher.hpp"

void MessageDispatcher::registerHandler(uint32_t messageType, std::function<void(const std::shared_ptr<IMessage>&, const std::string&, uint16_t)> handler) {
    handlers_[messageType] = handler;
}

void MessageDispatcher::dispatchMessage(const std::shared_ptr<IMessage>& message, const std::string& address, uint16_t port) {
    auto it = handlers_.find(message->getType());
    if (it != handlers_.end()) {
        it->second(message, address, port);
    } else {
        // Unknown message type ^^
    }
}
