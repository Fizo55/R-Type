#ifndef GAME_SERVER_HPP_
    #define GAME_SERVER_HPP_

    #include "engineCore.hpp"
    #include "engineScripting.hpp"

    #include "ServerNetwork.hpp"
    #include "gameArguments.hpp"

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

        private:
            bool _running;

            engine::Game _game;
            engine::EntityFactory _factory;

            Clock _clock;

            engine::ScriptOrchestrator _orchestrator;

            boost::asio::io_context _io;
            std::shared_ptr<NetworkServer> _server;
            std::thread _ioThread;
    };

#endif /* GAME_SERVER_HPP_ */
