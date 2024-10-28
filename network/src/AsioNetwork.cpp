#include "AsioNetwork.hpp"
#include <iostream>

AsioNetwork::AsioNetwork(const std::string& ipAddress, uint16_t port)
    : socket_(ioContext_), isRunning_(false)
{
    try {
        boost::asio::ip::address ip = boost::asio::ip::make_address(ipAddress);
        boost::asio::ip::udp::endpoint endpoint(ip, port);
        socket_.open(endpoint.protocol());
        socket_.bind(endpoint);
    } catch (const boost::system::system_error& e) {
        std::cerr << "Failed to bind to " << ipAddress << ":" << port << " - " << e.what() << "\n";
        throw;
    }
}

AsioNetwork::~AsioNetwork()
{
    stop();
}

void AsioNetwork::start()
{
    if (isRunning_)
        return;

    isRunning_ = true;

    startReceive();

    ioThread_ = std::thread([this]() { runIoContext(); });
}

void AsioNetwork::stop()
{
    if (!isRunning_)
        return;

    isRunning_ = false;

    ioContext_.stop();

    if (ioThread_.joinable())
        ioThread_.join();
}

void AsioNetwork::send(const std::vector<uint8_t>& data, const std::string& address, uint16_t port)
{
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(address), port);
    socket_.async_send_to(
        boost::asio::buffer(data),
        endpoint,
        [](const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/) {
            // If there is errors we can debug
        });
}

void AsioNetwork::setReceiveHandler(std::function<void(const std::vector<uint8_t>&, const std::string&, uint16_t)> handler)
{
    receiveHandler_ = handler;
}

void AsioNetwork::setClientConnectedHandler(std::function<void(const std::string&, uint16_t)> handler)
{
    clientConnectedHandler_ = handler;
}

void AsioNetwork::setClientDisconnectedHandler(std::function<void(const std::string&, uint16_t)> handler)
{
    clientDisconnectedHandler_ = handler;
}

void AsioNetwork::runIoContext()
{
    while (isRunning_) {
        try {
            ioContext_.run();
        } catch (const std::exception& e) {
            std::cerr << "IO context error: " << e.what() << std::endl;
        }
    }
}

void AsioNetwork::startReceive() {
    socket_.async_receive_from(
        boost::asio::buffer(recvBuffer_),
        remoteEndpoint_,
        [this](const boost::system::error_code& error, std::size_t bytesTransferred) {
            if (!error && bytesTransferred > 0) {
                std::vector<uint8_t> data(recvBuffer_.begin(), recvBuffer_.begin() + bytesTransferred);
                std::string address = remoteEndpoint_.address().to_string();
                uint16_t port = remoteEndpoint_.port();

                std::string clientKey = address + ":" + std::to_string(port);
                if (connectedClients_.find(clientKey) == connectedClients_.end()) {
                    connectedClients_.insert(clientKey);
                    if (clientConnectedHandler_)
                        clientConnectedHandler_(address, port);
                }

                if (receiveHandler_)
                    receiveHandler_(data, address, port);
            }

            if (isRunning_)
                startReceive();
        });
}