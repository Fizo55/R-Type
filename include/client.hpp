#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include "engineCore.hpp"
    #include "engineGraphic.hpp"
    #include "MessageDispatcher.hpp"
    #include "MessageFactory.hpp"
    #include "INetwork.hpp"
    #include "AsioNetwork.hpp"
    #include "PlayerInputMessage.hpp"
    #include <boost/asio.hpp>
    #include <memory>
    

    class client {
        public:
            client();
            ~client();

            void event(void);
            void update(void);
            void draw(void);

            void mainloop(void);

            void sendAction(grw::event &event);

            void connect(const std::string &host, int port);

        private:
            bool _running;

            int _port;
            std::string _host;

            engine::Game _game;
            engine::EntityFactory _factory;

            engine::displayManager _displayManager;

            grw::clock _clock;

            unsigned int _gameWindow;

            std::map<unsigned long, grw::event> _events;
            // std::unique_ptr<grw::event> _algo_events; // TODO adapter à l'énum de l'algo

            std::shared_ptr<INetwork> network_;

    };

#endif /* CLIENT_HPP_ */
