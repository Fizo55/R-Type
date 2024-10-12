#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

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

#endif /* CLIENT_HPP_ */
