// MessageFactory.hpp

#ifndef MESSAGEFACTORY_HPP
#define MESSAGEFACTORY_HPP

#include "IMessage.hpp"
#include <memory>
#include <unordered_map>
#include <functional>
#include <cstdint>

class MessageFactory {
public:
    static std::shared_ptr<IMessage> createMessage(uint32_t messageType);

    template<typename T>
    static void registerMessageType(uint32_t messageType);

private:
    static std::unordered_map<uint32_t, std::function<std::shared_ptr<IMessage>()>>& getCreators();
};

#endif
