#include "tcp_server.hpp"
#include <iostream>

TCPServer::TCPServer(asio::io_context& io_context, unsigned short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void TCPServer::start_accept() {
    tcp::socket socket(acceptor_.get_io_context());
    acceptor_.async_accept(socket, [this, &socket](const std::error_code& error) {
        handle_accept(std::move(socket), error);
    });
}

void TCPServer::handle_accept(tcp::socket socket, const std::error_code& error) {
    if (!error) {
        handle_connect_request(socket);
    }
    start_accept();
}

void TCPServer::handle_connect_request(tcp::socket& socket) {
    char buffer[1024];
    asio::read(socket, asio::buffer(buffer, sizeof(MessageHeader) + sizeof(ConnectRequest)));

    MessageHeader header;
    Protocol::deserializeHeader(buffer, header);
    
    if (header.messageID == 0x0001) {
        ConnectRequest request;
        std::memcpy(&request, buffer + sizeof(MessageHeader), sizeof(ConnectRequest));

        std::cout << "Received connection request from player: " << request.playerName << std::endl;

        ConnectResponse response = { static_cast<uint16_t>(clients_.size() + 1) };
        char responseBuffer[sizeof(MessageHeader) + sizeof(ConnectResponse)];
        MessageHeader responseHeader = { 0x0002, header.sequenceNo, response.clientID, 0 };

        Protocol::serializeHeader(responseBuffer, responseHeader);
        std::memcpy(responseBuffer + sizeof(MessageHeader), &response, sizeof(ConnectResponse));

        asio::write(socket, asio::buffer(responseBuffer, sizeof(responseBuffer)));
        clients_[response.clientID] = std::move(socket);

        std::cout << "Client " << response.clientID << " connected." << std::endl;
    }
}
