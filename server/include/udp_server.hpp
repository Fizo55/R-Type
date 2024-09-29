#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <asio.hpp>
#include <unordered_map>
#include "protocol.hpp"

using asio::ip::udp;

class UDPServer {
public:
    UDPServer(asio::io_context& io_context, unsigned short port);
    void start_receive();

private:
    void handle_receive(const std::error_code& error, std::size_t bytes_transferred);

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    char recv_buffer_[1024];

    std::unordered_map<int, udp::endpoint> clients_;
};

#endif // UDP_SERVER_HPP
