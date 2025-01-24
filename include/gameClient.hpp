#ifndef GAME_CLIENT_HPP_
    #define GAME_CLIENT_HPP_

    #include <mutex>
    #include <condition_variable>

    #include <boost/asio.hpp>
    #include "engineCore.hpp"
    #include "engineGraphic.hpp"
    #include "engineScripting.hpp"

    #include "gameArguments.hpp"

    #include <string>

    #include <thread>

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

            void handle_read_size(const boost::system::error_code& ec, std::size_t bytes_transferred);
            void handle_write_ack(const boost::system::error_code& ec, std::size_t bytes_transferred);
            void start_read_size();
            void start_read_object();
            void start_write_ack();
            void do_read();
            void start_read_until_delimiter();

        private:
            enum class ReadState {
                READING_SIZE,
                READING_OBJECT,
                SENDING_ACK,
                READING_UNTIL_DELIMITER
            };
            
            ReadState _readState = ReadState::READING_SIZE;
            std::array<char, 4> _sizeBuffer;
            std::string _objectBuffer;
            uint16_t _objectSize = 0;

            bool _running;

            engine::Game _game;
            engine::EntityFactory _factory;

            engine::displayManager _displayManager;

            grw::clock _clock;

            unsigned int _gameWindow;

            engine::ScriptOrchestrator _orchestrator;

            boost::asio::io_context _ioContext;
            std::unique_ptr<boost::asio::ip::tcp::socket> _tcpSocket;

            std::string _delimiter;

            bool _loginCompleted;
            std::mutex _loginMutex;
            std::condition_variable _loginCondVar;

            std::thread _ioThread;
    };

#endif /* GAME_CLIENT_HPP_ */
