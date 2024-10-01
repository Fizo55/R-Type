#include "engineGraphic.hpp"

int main(void)
{
    unsigned long dt;
    grw::mask theMask = grw::mask(0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    grw::windowHandler winHandler = grw::windowHandler();
    grw::clock clock;
    grw::window *win0 = new grw::window("test 0", grw::videoMode(engine_math::vector2(800, 600), engine_math::vector2(0, 0), 32, grw::videoMode::CLOSABLE | grw::videoMode::ACCELERATION | grw::videoMode::VSYNC));
    grw::window *win1 = new grw::window("test 1", grw::videoMode(engine_math::vector2(800, 600), engine_math::vector2(100, 100), 32, grw::videoMode::CLOSABLE | grw::videoMode::ACCELERATION | grw::videoMode::VSYNC));
    grw::window *win2 = new grw::window("test 2", grw::videoMode(engine_math::vector2(800, 600), engine_math::vector2(200, 200), 32, grw::videoMode::CLOSABLE | grw::videoMode::ACCELERATION | grw::videoMode::VSYNC));

    win0->getSurface()->setMask(theMask);
    win1->getSurface()->setMask(theMask);
    win2->getSurface()->setMask(theMask);

    winHandler.addWindow(win0);
    winHandler.addWindow(win1);
    winHandler.addWindow(win2);

    std::cout << theMask.createColor(0xff, 0x00, 0x00, 0xff) << std::endl;
    std::cout << (int)theMask.getR(theMask.createColor(0xff, 0x00, 0x00, 0xff)) << std::endl;

    while (!winHandler.isAllWindowClosed()) {
        winHandler.eventWindows();
        winHandler.updateWindows();
        win0->clear(theMask.createColor(0xff, 0xff, 0x00, 0xff));
        win1->clear(theMask.createColor(0x00, 0xff, 0xff, 0xff));
        win2->clear(theMask.createColor(0xff, 0x00, 0xff, 0xff));
        winHandler.drawWindows();
        dt = clock.tick(60);

        std::cout << dt << std::endl;
    }

    delete win0;
    delete win1;
    delete win2;
    return (0);
}
