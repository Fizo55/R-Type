#ifndef INETWORK_HPP
#define INETWORK_HPP

#include <functional>
#include <vector>
#include <string>
#include <cstdint>

class INetwork {
public:
    virtual ~INetwork() {}

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void send(const std::vector<uint8_t>& data, const std::string& address, uint16_t port) = 0;
    virtual void setReceiveHandler(std::function<void(const std::vector<uint8_t>& data, const std::string& address, uint16_t port)> handler) = 0;
    virtual void setClientConnectedHandler(std::function<void(const std::string& address, uint16_t port)> handler) = 0;
    virtual void setClientDisconnectedHandler(std::function<void(const std::string& address, uint16_t port)> handler) = 0;
};

#endif