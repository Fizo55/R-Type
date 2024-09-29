#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <asio.hpp>
#include <unordered_map>
#include "protocol.hpp"

using asio::ip::tcp;

class TCPServer {
public:
    TCPServer(asio::io_context& io_context, unsigned short port);
    void start_accept();

private:
    void handle_accept(tcp::socket socket, const std::error_code& error);
    void handle_connect_request(tcp::socket& socket);

    tcp::acceptor acceptor_;
    std::unordered_map<int, tcp::socket> clients_;
};

#endif // TCP_SERVER_HPP
