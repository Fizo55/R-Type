#ifndef GAME_CLIENT_HPP_
    #define GAME_CLIENT_HPP_

    #include "engineCore.hpp"
    #include "engineGraphic.hpp"
    #include "engineScripting.hpp"

    class client {
        public:
            client(const std::string &);
            ~client();

            void event(bool &isFlying);
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

            engine::ScriptOrchestrator _orchestrator;
    };

#endif /* GAME_CLIENT_HPP_ */
