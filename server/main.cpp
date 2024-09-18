#include <asio.hpp>
#include "GameServer.hpp"

int main() {
    try {
        asio::io_context io_context;
        GameServer server(io_context, 12345);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
