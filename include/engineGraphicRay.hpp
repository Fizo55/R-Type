#ifndef ENGINE_GRAPHIC_HPP_
#define ENGINE_GRAPHIC_HPP_

    #include <iostream>
    #include <memory>
    #include <map>
    #include <vector>
    #include <raylib.h>

    #include "engineMath.hpp"
    #include "engineExceptions.hpp"
    #include "engineCore.hpp"

/** Base abstractions **/

    namespace grw {

        class texture {
            public:
                texture(const engine_math::vector2<int> & = engine_math::vector2<int>(256, 256));
                texture(const std::string &filePath);
                ~texture();

                void draw(int x, int y);
                void draw_rect(const engine_math::vector2<int> &, const engine_math::vector2<int> &, unsigned int);
                void clear(unsigned int);
                void optimize(const texture &);

                Texture2D getTexture(void) const;
                const engine_math::vector2<int> &getSize(void) const;

            private:
                Texture2D _texture;
                engine_math::vector2<int> _size;
        };

        class sprite {
            public:
                sprite(const std::shared_ptr<texture> &, const engine_math::vector2<int> &, const engine_math::vector2<int> &);
                ~sprite();

                void setPosition(const engine_math::vector2<int> &);
                void setSize(const engine_math::vector2<int> &);
                std::shared_ptr<texture> &getTexture();

                const engine_math::vector2<int> &getPosition() const;
                const engine_math::vector2<int> &getSize() const;

                void draw() const;

            private:
                std::shared_ptr<texture> _spriteTexture;
                RenderTexture2D _renderTexture;
                engine_math::vector2<int> _position;
                engine_math::vector2<int> _size;
        };

        class event {
            public:
                enum eventTypes {
                    NO_EVENT = 0,
                    CLOSE = 1,
                    QUIT = 2
                };

                event(unsigned long type = 0, const engine_math::vector2<int> &position = engine_math::vector2<int>(0, 0), int key = -1);

                unsigned long type;
                engine_math::vector2<int> position;
                int key;
        };

        class clock {
            public:
                clock();

                unsigned long tick(int = -1);

            private:
                unsigned long _oldTime;
        };

        // videoMode for the window
        class videoMode {
            public:
                videoMode(const engine_math::vector2<int> &, const engine_math::vector2<int> &, unsigned int, unsigned long);
                ~videoMode() = default;

                enum videoFlags {
                    VSYNC = 1 << 0,
                    ACCELERATION = 1 << 1,
                    CLOSABLE = 1 << 2
                };

                engine_math::vector2<int> size;
                engine_math::vector2<int> position;

                unsigned int depth;
                unsigned long flags;
        };

        // Beautiful window abstraction for Raylib
        class window {
            public:
                window(const std::string & = "New Window", const videoMode & = videoMode(engine_math::vector2<int>(800, 600), engine_math::vector2<int>(0, 0), 32, 0));
                ~window();

                void clear(unsigned int color);
                void update(void);
                void draw(void);

                void close(void);

                void clearEvents(void);
                void registerEvent(const event &);
                unsigned char hasEvent(unsigned long);
                const event getEvent(unsigned long) const;


                unsigned char isClosed(void);
                void addSprite(const std::string &, const std::shared_ptr<texture> &);
                void addSprite(const std::string &);
            private:
                unsigned char _closed;

                std::string _title;
                videoMode _videoMode;

                std::vector<sprite> _sprites;

                int _windowId;

                std::map<unsigned long, event> _events;
        };

        class windowHandler {
            public:
                windowHandler();
                ~windowHandler();

                void addWindow(window *);
                window *getWindow(unsigned int);
                void closeWindow(unsigned int);

                unsigned char isAllWindowClosed(void);
                unsigned char isWindowClosed(unsigned int);
                event buildEvent();

                void eventWindows(void);
                void updateWindows(void);
                void drawWindows(void);
                void clearWindows(void);

            private:
                std::vector<window *> _windows;
        };
    };

    namespace engine {
        class displayManager {
            public:
                displayManager();

                void registerAsset(const std::string &, const std::string &);
                void registerAsset(const std::string &, const std::shared_ptr<grw::texture> &);

                void addWindow(grw::window *);

                bool event(void);
                void update(void);
                void draw(void);
                void clear(void);

                unsigned int createWindow(void);

                void useEntity(const Entity &, Registry &, unsigned int);
            private:
                grw::windowHandler _winHandler;
        };
    };

#endif /* !ENGINE_GRAPHIC_HPP_ */
