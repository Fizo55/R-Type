/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engineCore.hpp"
#include "client.hpp"
#include "engineComponents.hpp"

using namespace engine;

int getPort(char **av)
{
    int port;

    if (std::string(av[1]) != "-p")
        help();
    try {
        port = std::stoi(av[2]);
    } catch (std::exception &e) {
        help();
    }
    return port;
}

std::string getHost(char **av)
{
    std::string host;

    if (std::string(av[3]) != "-h")
        help();
    host = av[4];
    return host;
}

void help(void)
{
    std::cout << "Usage: ./client -p P -h H" << std::endl;
    std::cout << "P: Port" << std::endl;
    std::cout << "H: Host" << std::endl;
    exit(0);
}

int main(int ac, char **av)
{
    client game = client();
    int port;
    std::string host;

    if (ac != 5)
        help();
    
    port = getPort(av);
    host = getHost(av);
    try
    {
        game.connect(host, port);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    game.mainloop();
    return 0;
}
