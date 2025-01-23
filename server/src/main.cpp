#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "gameServer.hpp"
#include "ServerNetwork.hpp"

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
    std::string ipAddress = "0.0.0.0";

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

    boost::system::error_code ec;
    boost::asio::ip::make_address(ipAddress, ec);
    if (ec) {
        std::cerr << "Invalid IP address: " << ipAddress << "\n";
        return 84;
    }

    std::cout << "[Main] Starting server on IP=" << ipAddress 
              << " TCP=" << port << " UDP=" << (port+1) << "\n";

    boost::asio::io_context io;

    auto server = std::make_shared<NetworkServer>(io, ipAddress, port, port + 1);

    server->start();

    std::cout << "Server is running. Type 'exit' to stop.\n";

    std::thread ioThread([&io]() {
        io.run();
    });

    while (true) {
        std::string command;
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
    }

    server->stop();

    io.stop();
    if (ioThread.joinable()) {
        ioThread.join();
    }

    std::cout << "[Main] Server has been stopped.\n";
    return 0;
}
