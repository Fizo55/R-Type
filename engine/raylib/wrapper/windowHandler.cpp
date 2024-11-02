#include "engineGraphicRay.hpp"

using namespace grw;

windowHandler::windowHandler()
{
    // Constructor body, if needed
}

windowHandler::~windowHandler()
{
    this->_windows.clear();
}

void windowHandler::addWindow(window *win)
{
    this->_windows.push_back(win);
}

window *windowHandler::getWindow(unsigned int id)
{
    return this->_windows[id];
}

unsigned char windowHandler::isAllWindowClosed(void)
{
    for (const auto &win : this->_windows) {
        if (!win->isClosed())
            return 0;
    }
    return 1;
}

unsigned char windowHandler::isWindowClosed(unsigned int id)
{
    return (!this->_windows[id] || this->_windows[id]->isClosed());
}

/**
 * @brief Handle the events of the windows
 */
void windowHandler::eventWindows(void)
{
    for (const auto &win : this->_windows) {
        if (win->isClosed())
            continue;
        
        win->clearEvents();
        
        // Handling window close events
        if (WindowShouldClose()) {
            win->registerEvent(event(event::QUIT));
        }
    }
}

/**
 * @brief Close a specific window by ID
 * 
 * @param id The ID of the window to close
 */
void windowHandler::closeWindow(unsigned int id)
{
    if (this->_windows[id])
        this->_windows[id]->close();
}

/**
 * @brief Update all windows
 */
void windowHandler::updateWindows(void)
{
    for (const auto &win : this->_windows) {
        if (win->isClosed())
            continue;

        win->update();
    }

    // Clean up closed windows
    for (auto it = this->_windows.cbegin(); it != this->_windows.cend();) {
        if ((*it)->isClosed())
            this->_windows.erase(it++);
        else
            ++it;
    }
}

/**
 * @brief Draw all open windows
 */
void windowHandler::drawWindows(void)
{
    for (const auto &win : this->_windows) {
        if (win->isClosed())
            continue;
        win->draw();
    }
}

/**
 * @brief Clear all windows
 */
void windowHandler::clearWindows(void)
{
    for (const auto &win : this->_windows) {
        if (win->isClosed())
            continue;

        win->clear(0);
    }
}