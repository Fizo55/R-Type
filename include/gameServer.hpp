#ifndef GAME_SERVER_HPP_
    #define GAME_SERVER_HPP_

    #include "engineCore.hpp"
    #include "engineScripting.hpp"

    #include "ServerNetwork.hpp"
    #include "gameArguments.hpp"

    #include <deque>
    #include <thread>
    #include <boost/asio.hpp>


    class Clock {
        public:
            Clock();

            unsigned long tick(int = -1);
            unsigned long lastDT(void);

        private:
            unsigned long _oldTime;
            unsigned long _lastDT;
    };


    class server {
        public:
            server(const std::string &);
            ~server();

            void event(void);
            void update(void);

            int listen(const std::string &, uint16_t);

            void mainloop(void);

            void send_object(const std::vector<char>& payload);

            void start_send_object();

        private:
            bool _running;

            engine::Game _game;
            engine::EntityFactory _factory;

            Clock _clock;

            engine::ScriptOrchestrator _orchestrator;

            boost::asio::io_context _io;
            std::shared_ptr<NetworkServer> _server;
            std::thread _ioThread;

            std::mutex _sendMutex;
            std::deque<std::vector<char>> _sendQueue;
            bool _sendInProgress;
            std::string _delimiter = "\r\n\r\n";
    };

#endif /* GAME_SERVER_HPP_ */
