#ifndef ASIONETWORK_HPP
#define ASIONETWORK_HPP

#include "INetwork.hpp"
#include <boost/asio.hpp>
#include <thread>
#include <atomic>

class AsioNetwork : public INetwork {
public:
    AsioNetwork(uint16_t port);
    ~AsioNetwork();

    void start() override;
    void stop() override;

    void send(const std::vector<uint8_t>& data, const std::string& address, uint16_t port) override;

private:
    void doReceive();
    void handleReceive(const boost::system::error_code& error, std::size_t bytesTransferred);

    boost::asio::io_context ioContext_;
    boost::asio::ip::udp::socket socket_;
    std::thread ioThread_;
    std::atomic<bool> isRunning_;

    std::array<uint8_t, 1024> recvBuffer_;
    boost::asio::ip::udp::endpoint remoteEndpoint_;
};

#endif
