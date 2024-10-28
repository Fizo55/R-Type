#ifndef INETWORK_HPP
#define INETWORK_HPP

#include <functional>
#include <vector>
#include <string>
#include <cstdint>

class INetwork {
public:
    virtual ~INetwork() = default;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void send(const std::vector<uint8_t>& data, const std::string& address, uint16_t port) = 0;

    std::function<void(const std::vector<uint8_t>& data, const std::string& address, uint16_t port)> onReceive;
    std::function<void(const std::string& address, uint16_t port)> onClientConnected;
    std::function<void(const std::string& address, uint16_t port)> onClientDisconnected;
};

#endif