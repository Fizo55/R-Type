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
        class mask {
            public:
                mask(unsigned r = 0xff000000, unsigned int g = 0x00ff0000, unsigned int b = 0x0000ff00, unsigned int a = 0x000000ff);
                ~mask() = default;

                unsigned createColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
                unsigned char getR(unsigned int color);
                unsigned char getG(unsigned int color);
                unsigned char getB(unsigned int color);
                unsigned char getA(unsigned int color);

                unsigned int r;
                unsigned int g;
                unsigned int b;
                unsigned int a;
        };

        class texture {
            public:
                texture(const engine_math::vector2<int> & = engine_math::vector2<int>(256, 256), unsigned int = 32, const mask & = mask());
                texture(const std::string &filePath);
                ~texture();

                void blit(const texture &, const engine_math::vector2<int> &, const engine_math::vector2<int> &);
                void draw_rect(const engine_math::vector2<int> &, const engine_math::vector2<int> &, unsigned int);
                void clear(unsigned int);
                void optimize(const texture &);
                void setMask(const mask &);

                Texture2D getTexture(void) const;
                const engine_math::vector2<int> &getSize(void) const;
                const mask &getMask(void) const;

            private:
                Texture2D _texture;
                RenderTexture2D _renderTexture;
                engine_math::vector2<int> _size;
                unsigned int _depth;
                mask _mask;
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

                void draw(const std::shared_ptr<texture> &) const;

            private:
                std::shared_ptr<texture> _spriteTexture;
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

                void blit(const texture &, const engine_math::vector2<int> &, const engine_math::vector2<int> &);
                void clear(unsigned int color);
                void update(void);
                void draw(void);

                void close(void);

                void clearEvents(void);
                void registerEvent(const event &);
                unsigned char hasEvent(unsigned long);
                const event getEvent(unsigned long) const;

                std::shared_ptr<texture> &getSurface(void);
                const std::shared_ptr<texture> &getSurface(void) const;


                unsigned char isClosed(void);
            private:
                unsigned char _closed;

                std::string _title;
                videoMode _videoMode;

                std::shared_ptr<texture> _texture;

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

                std::map<unsigned int, std::vector<grw::sprite>> _rendering;
                std::map<std::string, std::shared_ptr<grw::texture>> _textures;
        };
    };

#endif /* !ENGINE_GRAPHIC_HPP_ */
