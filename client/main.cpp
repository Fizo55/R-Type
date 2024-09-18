#include "engineGraphic.hpp"

int main(void)
{
    unsigned long dt;
    grw::windowHandler winHandler = grw::windowHandler();
    grw::clock clock;
    grw::window *win0 = new grw::window("test 0", grw::videoMode(engine_math::vector2(800, 600), engine_math::vector2(0, 0), 32, grw::videoMode::CLOSABLE));
    grw::window *win1 = new grw::window("test 1", grw::videoMode(engine_math::vector2(800, 600), engine_math::vector2(100, 100), 32, grw::videoMode::CLOSABLE));
    grw::window *win2 = new grw::window("test 2", grw::videoMode(engine_math::vector2(800, 600), engine_math::vector2(200, 200), 32, grw::videoMode::CLOSABLE));

    winHandler.addWindow(win0);
    winHandler.addWindow(win1);
    winHandler.addWindow(win2);

    while (!winHandler.isAllWindowClosed()) {
        winHandler.eventWindows();
        winHandler.updateWindows();
        win0->clear(0xff0000ff);
        win1->clear(0x00ff00ff);
        win2->clear(0x0000ffff);
        winHandler.drawWindows();
        dt = clock.tick(60);

        std::cout << dt << std::endl;
    }

    delete win0;
    delete win1;
    delete win2;
    return (0);
}
