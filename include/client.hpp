#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include "engineCore.hpp"
    #include "engineGraphic.hpp"
    #include <memory>

    class client {
        public:
            client();
            ~client();

            void event(void);
            void update(void);
            void draw(void);

            void mainloop(void);

        private:
            bool _running;

            engine::Game _game;
            engine::EntityFactory _factory;

            engine::displayManager _displayManager;

            grw::clock _clock;

            unsigned int _gameWindow;

            std::map<unsigned, grw::event> _events;
            // std::unique_ptr<grw::event> _algo_events; // TODO adapter à l'énum de l'algo
    };

#endif /* CLIENT_HPP_ */
