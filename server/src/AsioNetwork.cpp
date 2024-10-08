#include "AsioNetwork.hpp"
#include <iostream>

AsioNetwork::AsioNetwork(uint16_t port)
    : socket_(ioContext_), isRunning_(false) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), port);
    socket_.open(endpoint.protocol());
    socket_.bind(endpoint);
}

AsioNetwork::~AsioNetwork() {
    stop();
}

void AsioNetwork::start() {
    isRunning_ = true;
    doReceive();
    ioThread_ = std::thread([this]() { ioContext_.run(); });
}

void AsioNetwork::stop() {
    isRunning_ = false;
    socket_.close();
    ioContext_.stop();
    if (ioThread_.joinable()) {
        ioThread_.join();
    }
}

void AsioNetwork::send(const std::vector<uint8_t>& data, const std::string& address, uint16_t port) {
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::make_address(address), port);
    socket_.async_send_to(
        boost::asio::buffer(data), endpoint,
        [](const boost::system::error_code& /*error*/, std::size_t /*bytesTransferred*/) {
            // This is send result but we don't need to do anything here
        });
}

void AsioNetwork::doReceive() {
    socket_.async_receive_from(
        boost::asio::buffer(recvBuffer_), remoteEndpoint_,
        [this](const boost::system::error_code& error, std::size_t bytesTransferred) {
            handleReceive(error, bytesTransferred);
        });
}

void AsioNetwork::handleReceive(const boost::system::error_code& error, std::size_t bytesTransferred) {
    if (!error && bytesTransferred > 0) {
        std::vector<uint8_t> data(recvBuffer_.begin(), recvBuffer_.begin() + bytesTransferred);
        if (onReceive) {
            onReceive(data, remoteEndpoint_.address().to_string(), remoteEndpoint_.port());
        }
    } else {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }

    if (isRunning_) {
        doReceive();
    }
}
