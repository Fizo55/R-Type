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

/**
 * @brief Build an event from a SDL_Event
 * 
 * @param eventSDL The SDL_Event to build the event from
 */
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
    if (eventType == SDL_KEYDOWN) {
            switch (eventSDL->key.keysym.sym) {
        case SDLK_UP:
            return (event::UP);
        case SDLK_DOWN:
            return (event::DOWN);
        case SDLK_LEFT:
            return (event::LEFT);
        case SDLK_RIGHT:
            return (event::RIGHT);
        case SDLK_SPACE:
            return (event::SPACE);
        default:
            return (event(event::KEY_PRESSED));
        }
    }
    if (eventType == SDL_KEYUP) {
        return (event(event::KEY_RELEASED));
    }
    return (event());
}

/**
 * @brief Handle the events of the windows
 */
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