#include "engineGraphic.hpp"

using namespace grw;

windowHandler::windowHandler()
{

};

windowHandler::~windowHandler()
{
    this->_windows.clear();
}

void windowHandler::addWindow(window *win)
{
    this->_windows[win->getId()] = win;
}

window *windowHandler::getWindow(unsigned int id)
{
    if (!this->hasWindow(id))
        return (nullptr);
    return (this->_windows[id]);
}

unsigned char windowHandler::isAllWindowClosed(void)
{
    for (const auto &myPair : this->_windows) {
        if (!myPair.second->isClosed())
            return (0);
    }
    return (1);
}

unsigned char windowHandler::hasWindow(unsigned int id)
{
    return (this->_windows.find(id) != this->_windows.end());
}

unsigned char windowHandler::isWindowClosed(unsigned int id)
{
    return (!this->hasWindow(id) || this->_windows[id]->isClosed());
}

event windowHandler::buildEvent(SDL_Event *eventSDL)
{
    unsigned int eventType = 0;

    if (eventSDL->window.windowID)
        eventType = eventSDL->type != SDL_WINDOWEVENT ? eventSDL->type : eventSDL->window.event;
    else
        eventType = eventSDL->type;

    if (eventType == SDL_QUIT)
        return (event(event::QUIT));
    if (eventType == SDL_WINDOWEVENT_CLOSE)
        return (event(event::CLOSE));
    return (event());
}

void windowHandler::eventWindows(void)
{
    SDL_Event eventSDL;
    unsigned int windowId;

    for (const auto &myPair : this->_windows) {
        if (myPair.second->isClosed())
            continue;

        myPair.second->clearEvents();
    }

    while (SDL_PollEvent(&eventSDL) != 0) {
        windowId = eventSDL.window.windowID;

        if (this->hasWindow(windowId)) {
            this->getWindow(windowId)->registerEvent(this->buildEvent(&eventSDL));

        } else {
            for (const auto &myPair : this->_windows) {
                if (myPair.second->isClosed())
                    continue;

                myPair.second->registerEvent(this->buildEvent(&eventSDL));
            }
        }
    }
}

void windowHandler::closeWindow(unsigned int id)
{
    if (this->hasWindow(id))
        this->_windows[id]->close();
}

void windowHandler::updateWindows(void)
{
    for (const auto &myPair : this->_windows) {
        if (myPair.second->isClosed())
            continue;

        myPair.second->update();
    }

    for (auto it = this->_windows.cbegin(); it != this->_windows.cend();) {
        if (it->second->isClosed())
            this->_windows.erase(it++);
        else
            ++it;
    }
}

void windowHandler::drawWindows(void)
{
    for (const auto &myPair : this->_windows) {
        if (myPair.second->isClosed())
            continue;

        myPair.second->draw();
    }
}