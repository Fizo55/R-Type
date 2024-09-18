#ifndef GAME_SERVER_HPP
#define GAME_SERVER_HPP

#include <asio.hpp>
#include <vector>
#include <array>
#include <string>

class GameServer {
public:
    GameServer(asio::io_context& io_context, int port);

    void start_receive();

private:
    void handle_message(const std::string& message);

    void send_game_state();

    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
    std::vector<asio::ip::udp::endpoint> client_endpoints_;
};

#endif // GAME_SERVER_HPP
