/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engineCore.hpp"
#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineScripting.hpp"
#include <cstdlib>

using namespace engine;

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
        std::cout << "  -h                 Show help message\n";
        std::cout << "  --port=<port>      Set the port number (TCP)\n";
        std::cout << "  --ip=<ip_address>  Set the IP address to bind\n";
        return 0;
    }

    uint16_t port = 1337;
    std::string ipAddress = "127.0.0.1";

    if (parser.hasArg("--port")) {
        try {
            port = static_cast<uint16_t>(std::any_cast<short>(parser.getArg("--port").value));
        } catch (const std::bad_any_cast& e) {
            std::cerr << "Invalid port number provided for --port.\n";
            return 84;
        }
    }

    if (parser.hasArg("--ip")) {
        ipAddress = std::any_cast<std::string>(parser.getArg("--ip").value);
    }

    client game = client("r-type/game.yml");

    game.connect(ipAddress, port);
    game.mainloop();
    return 0;
}
