#include "MessageFactory.hpp"

std::unordered_map<uint32_t, std::function<std::shared_ptr<IMessage>()>>& MessageFactory::getCreators() {
    static std::unordered_map<uint32_t, std::function<std::shared_ptr<IMessage>()>> creators;
    return creators;
}

std::shared_ptr<IMessage> MessageFactory::createMessage(uint32_t messageType) {
    auto& creators = getCreators();
    auto it = creators.find(messageType);
    if (it != creators.end()) {
        return it->second();
    }
    return nullptr;
}

template<typename T>
void MessageFactory::registerMessageType(uint32_t messageType) {
    auto& creators = getCreators();
    creators[messageType] = []() { return std::make_shared<T>(); };
}

// template void MessageFactory::registerMessageType<Class>(uint32_t);
