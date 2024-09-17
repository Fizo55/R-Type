#include "engineGraphic.hpp"

int main(void)
{
    grw::window window;
    grw::clock clock;
    window.draw();

    while (1) {
        auto dt = clock.tick(60);

        std::cout << dt << std::endl;
    }
    return (0);
}
