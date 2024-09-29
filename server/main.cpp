#include "tcp_server.hpp"
#include "udp_server.hpp"
#include <asio.hpp>
#include <thread>

int main() {
    try {
        asio::io_context io_context;

        TCPServer tcp_server(io_context, 12345);
        UDPServer udp_server(io_context, 12346);

        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([&io_context]() {
                io_context.run();
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
