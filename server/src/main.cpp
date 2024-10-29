#include <iostream>
#include "RTypeServer.hpp"
#include "AsioNetwork.hpp"
#include "RTypeGameLogic.hpp"
#include "ConsoleLogger.hpp"
#include "MessageFactory.hpp"
#include "PlayerInputMessage.hpp"
#include "ClientHelloMessage.hpp"
#include "GameStateUpdateMessage.hpp"
#include "AcknowledgmentMessage.hpp"
#include "ServerWelcomeMessage.hpp"
#include "gameServer.hpp"

int main(int argc, char **argv)
{
    argumentParser parser = argumentParser(0, 0);
    argumentTypeFactory factory = argumentTypeFactory();

    factory.registerBuildRoute(argument::STRING, argument::base_string_builder);
    factory.registerBuildRoute(argument::FLOAT, argument::base_float_builder);
    factory.registerBuildRoute(argument::LONG, argument::base_long_builder);
    factory.registerBuildRoute(argument::INT, argument::base_int_builder);
    factory.registerBuildRoute(argument::SHORT, argument::base_short_builder);
    factory.registerBuildRoute(argument::CHAR, argument::base_char_builder);

    parser.attachFactory(factory);
    parser.registerArg(argument("-h", argument::HELP));
    parser.registerArg(argument("--port", argument::ARGUMENT_TYPE::EQUAL, argument::SHORT));
    parser.registerArg(argument("--ip", argument::ARGUMENT_TYPE::EQUAL, argument::STRING));

    parser.parseArgs(argc, argv);

    if (parser.hasArg("-h")) {
        std::cout << "Usage: " << argv[0] << " [options]\n";
        std::cout << "Options:\n";
        std::cout << "  -h               Show help message\n";
        std::cout << "  --port=<port>    Set the port number\n";
        std::cout << "  --ip=<ip_address>  Set the IP address to bind\n";
        return 0;
    }

    uint16_t port = 1337;
    std::string ipAddress = "0.0.0.0";

    if (parser.hasArg("--port")) {
        try {
            port = static_cast<uint16_t>(std::any_cast<short>(parser.getArg("--port").value));
        } catch (const std::bad_any_cast& e) {
            std::cerr << "Invalid port number provided for --port.\n";
            return 84;
        }
    }

    if (parser.hasArg("--ip"))
        ipAddress = std::any_cast<std::string>(parser.getArg("--ip").value);

    boost::system::error_code ec;
    boost::asio::ip::make_address(ipAddress, ec);
    if (ec) {
        std::cerr << "Invalid IP address: " << ipAddress << "\n";
        return 84;
    }

    MessageFactory::registerMessageType<PlayerInputMessage>(0x10);
    MessageFactory::registerMessageType<ClientHelloMessage>(0x01);
    MessageFactory::registerMessageType<GameStateUpdateMessage>(0x11);
    MessageFactory::registerMessageType<AcknowledgmentMessage>(0xFE);
    MessageFactory::registerMessageType<ServerWelcomeMessage>(0x02);

    auto logger = std::make_shared<ConsoleLogger>();
    auto network = std::make_shared<AsioNetwork>(ipAddress, port);
    auto gameLogic = std::make_shared<RTypeGameLogic>(network, logger);

    RTypeServer server(network, logger);

    server.start();

    logger->logInfo("Server is running. Type 'exit' to stop.");

    while (true) {
        std::string command;
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
    }

    server.stop();
    logger->logInfo("Server has been stopped.");

    return 0;
}
