#ifndef GAME_CLIENT_HPP_
    #define GAME_CLIENT_HPP_

    #include <mutex>
    #include <condition_variable>

    #include <boost/asio.hpp>
    #include "engineCore.hpp"
    #include "engineGraphic.hpp"
    #include "engineScripting.hpp"

    #include "gameArguments.hpp"

    class client {
        public:
            client(const std::string &);
            ~client();

            void event(void);
            void update(void);
            void draw(void);

            void mainloop(void);

            void connect(const std::string &serverIp, unsigned short serverPort);
            void login(void);

        private:
            bool _running;

            engine::Game _game;
            engine::EntityFactory _factory;

            engine::displayManager _displayManager;

            grw::clock _clock;

            unsigned int _gameWindow;

            engine::ScriptOrchestrator _orchestrator;

            boost::asio::io_context _ioContext;
            std::unique_ptr<boost::asio::ip::tcp::socket> _tcpSocket;

            bool _loginCompleted;
            std::mutex _loginMutex;
            std::condition_variable _loginCondVar;
    };

#endif /* GAME_CLIENT_HPP_ */
