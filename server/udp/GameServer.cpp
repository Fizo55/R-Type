#include "GameServer.hpp"
#include <iostream>

GameServer::GameServer(asio::io_context& io_context, int port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
    start_receive();
}

void GameServer::start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                std::string message(recv_buffer_.begin(), recv_buffer_.begin() + bytes_recvd);
                handle_message(message);
                send_game_state();
            }
            start_receive();
        });
}

void GameServer::handle_message(const std::string& message) {
    std::cout << "Received: " << message << std::endl;
}

void GameServer::send_game_state() {
    std::string game_state = "Current game state";
    for (const auto& client_endpoint : client_endpoints_) {
        socket_.async_send_to(
            asio::buffer(game_state), client_endpoint,
            [this](std::error_code ec, std::size_t) {
                if (ec) {
                    std::cerr << "Failed to send game state: " << ec.message() << std::endl;
                }
            });
    }
}
