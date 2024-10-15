#include "Server.hpp"
#include "AsioNetwork.hpp"
#include "RTypeGameLogic.hpp"
#include "ConsoleLogger.hpp"
#include "MessageFactory.hpp"
#include "PlayerInputMessage.hpp"

int main() {
    MessageFactory::registerMessageType<PlayerInputMessage>(MessageType::PlayerInput);

    uint16_t port = 1337;

    auto logger = std::make_shared<ConsoleLogger>();
    auto network = std::make_shared<AsioNetwork>(port);
    auto gameLogic = std::make_shared<RTypeGameLogic>(network, logger);

    Server server(network, gameLogic, logger);

    server.start();

    while (true) {
        std::string command;
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
    }

    server.stop();

    return 0;
}
