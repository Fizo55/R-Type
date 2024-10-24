#ifndef GAME_CLIENT_HPP_
    #define GAME_CLIENT_HPP_

    #include "engineCore.hpp"
    #include "engineGraphic.hpp"

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
    };

#endif /* GAME_CLIENT_HPP_ */
