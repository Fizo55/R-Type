#ifndef ASIONETWORK_HPP
#define ASIONETWORK_HPP

#include "INetwork.hpp"
#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <set>

class AsioNetwork : public INetwork {
public:
    AsioNetwork(const std::string& ipAddress, uint16_t listenPort);
    ~AsioNetwork();

    void start() override;
    void stop() override;
    void send(const std::vector<uint8_t>& data, const std::string& address, uint16_t port) override;

    void setReceiveHandler(std::function<void(const std::vector<uint8_t>& data, const std::string& address, uint16_t port)> handler) override;
    void setClientConnectedHandler(std::function<void(const std::string& address, uint16_t port)> handler) override;
    void setClientDisconnectedHandler(std::function<void(const std::string& address, uint16_t port)> handler) override;

private:
    void runIoContext();
    void startReceive();

    uint16_t listenPort_;
    std::atomic<bool> isRunning_;
    std::thread ioThread_;
    boost::asio::io_context ioContext_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remoteEndpoint_;
    std::array<uint8_t, 65536> recvBuffer_;

    std::function<void(const std::vector<uint8_t>&, const std::string&, uint16_t)> receiveHandler_;
    std::function<void(const std::string&, uint16_t)> clientConnectedHandler_;
    std::function<void(const std::string&, uint16_t)> clientDisconnectedHandler_;

    std::set<std::string> connectedClients_;
};
#endif
