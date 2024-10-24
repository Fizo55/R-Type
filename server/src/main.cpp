#include <iostream>
#include "Server.hpp"
#include "AsioNetwork.hpp"
#include "RTypeGameLogic.hpp"
#include "ConsoleLogger.hpp"
#include "MessageFactory.hpp"
#include "PlayerInputMessage.hpp"
#include "gameServer.hpp"

int main(int argc, char **argv) {
    argumentParser parser = argumentParser(1, 4);
    argumentTypeFactory factory = argumentTypeFactory();

    factory.registerBuildRoute(argument::STRING, argument::base_string_builder);
    factory.registerBuildRoute(argument::FLOAT, argument::base_float_builder);
    factory.registerBuildRoute(argument::LONG, argument::base_long_builder);
    factory.registerBuildRoute(argument::INT, argument::base_int_builder);
    factory.registerBuildRoute(argument::SHORT, argument::base_short_builder);
    factory.registerBuildRoute(argument::CHAR, argument::base_char_builder);

    parser.attachFactory(factory);
    parser.registerUnamedArg(argument::STRING);
    parser.registerArg(argument("-h", argument::HELP));
    parser.registerArg(argument("--port", argument::ARGUMENT_TYPE::EQUAL, argument::SHORT));
    parser.registerArg(argument("-p", argument::ARGUMENT_TYPE::VALUE, argument::SHORT));
    parser.registerArg(argument("-c", argument::ARGUMENT_TYPE::SINGLE));

    parser.parseArgs(argc, argv);

    if (parser.hasArg("-h")) {
        std::cout << "help" << std::endl;
        return (0);
    }
    if (parser.hasArg("-c")) {
        std::cout << "-c" << std::endl;
        return (0);
    }
    if (parser.hasArg("-p")) {
        std::cout << std::any_cast<short>(parser.getArg("-p").value) << std::endl;
        return (0);
    }
    if (parser.hasArg("--port")) {
        std::cout << std::any_cast<short>(parser.getArg("--port").value) << std::endl;
        return (0);
    }

    std::cout << std::any_cast<std::string>(parser.getUnnamedArgs()[0]) << std::endl;

    return (0);

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
