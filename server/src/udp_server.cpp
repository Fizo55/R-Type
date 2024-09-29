#include "udp_server.hpp"
#include <iostream>

UDPServer::UDPServer(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)) {
    start_receive();
}

void UDPServer::start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        [this](const std::error_code& error, std::size_t bytes_transferred) {
            handle_receive(error, bytes_transferred);
        });
}

void UDPServer::handle_receive(const std::error_code& error, std::size_t bytes_transferred) {
    if (!error && bytes_transferred > 0) {
        MessageHeader header;
        Protocol::deserializeHeader(recv_buffer_, header);

        switch (header.messageID) {
            case 0x1001: {
                PlayerMove move;
                Protocol::deserializePlayerMove(recv_buffer_ + sizeof(MessageHeader), move);
                std::cout << "Player " << header.clientID << " moved to (" << move.posX << ", " << move.posY << ").\n";
                break;
            }
            case 0x1002: {
                FireMissile missile;
                Protocol::deserializeFireMissile(recv_buffer_ + sizeof(MessageHeader), missile);
                std::cout << "Player " << header.clientID << " fired missile at (" << missile.posX << ", " << missile.posY << ").\n";
                break;
            }
            default:
                std::cerr << "Unknown message ID: " << header.messageID << std::endl;
        }
    }
    start_receive();
}
