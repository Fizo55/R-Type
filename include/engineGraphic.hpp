#ifndef ENGINE_GRAPHIC_HPP_
    #define ENGINE_GRAPHIC_HPP_

    #include <iostream>
    #include <memory>
    #include <SDL2/SDL_surface.h>
    #include <SDL2/SDL_video.h>

    #include "engineMath.hpp"

    /** Base abstractions **/

    namespace grw {
        class texture {
            public:
                texture(const engine_math::vector2<int> &size = engine_math::vector2<int>(256, 256), unsigned int depht = 32, unsigned char alpha = 1);
                texture(SDL_Surface *surface);
                ~texture();

                void blit(const texture &, const engine_math::vector2<int> &, const engine_math::vector2<int> &);
                void clear(const SDL_Color &color);

            private:
                SDL_Surface *_texture;
        };

        class event {
            public:
                event(unsigned long type, const engine_math::vector2<int> &position, int key);

                unsigned long type;
                engine_math::vector2<int> position;
                int key;
        };

        class clock {
            public:
                clock();

                unsigned long tick(int fps);

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
                    ACCELERATION = 1 << 1
                };

                engine_math::vector2<int> size;
                engine_math::vector2<int> position;

                unsigned int depht;
                unsigned long flags;
        };

        // Beautiful window abstraction for SDL2
        class window {
            public:
                window(const std::string & = "New Window", const videoMode & = videoMode(engine_math::vector2<int>(800, 600), engine_math::vector2<int>(0, 0), 32, 0));
                ~window();

                void blit(const texture &other, const engine_math::vector2<int> &, const engine_math::vector2<int> &);
                void clear(const SDL_Color &color);
                void draw();

            private:
                std::string _title;
                videoMode _videoMode;
                std::unique_ptr<texture> _texture;
                SDL_Window *_window;
        };
    };

    namespace engine {

    };

#endif /* !ENGINE_GRAPHIC_HPP_ */
