#include "client.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"

using namespace engine;

client::client()
  : _running(false)
{
    this->_factory.registerComponent<engine_components::Position>();
    this->_factory.registerComponent<engine_components::Sprite>();

    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("position", engine_components::Position::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("sprite", engine_components::Sprite::buildComponent));

    this->_displayManager.registerAsset("sprite0", "assets/images/error.png");

    this->_game.registerObject("assets/objects/test.yml");
    this->_game.registerScene("assets/scenes/main_scene.yml");

    this->_game.addFactory(&this->_factory);

    this->_game.loadScene("scene0");

    this->_gameWindow = this->_displayManager.createWindow();
}

client::~client()
{

}

/**
 * @brief Connect to the server
 * 
 * @param host The host to connect to
 * @param port The port to connect to
 */

void client::connect(const std::string &host, int port)
{
    auto network = std::make_shared<AsioNetwork>(port);
    network_ = network;
}


/**
 * @brief Send an action to the server
 * 
 * @param event The event to send
 */
void client::sendAction(grw::event &event)
{
    auto gameStateMessage = std::make_shared<GameStateUpdateMessage>();

    std::vector<uint8_t> outData;
    gameStateMessage->serialize(outData);

    network_->send(outData, _host, _port);
}

void client::event(void)
{
    this->_running = !this->_displayManager.event();
    _events = this->_displayManager.getEvents(this->_gameWindow);
    for (auto &event : _events) {
        if (event.second.type == grw::event::QUIT || event.second.type == grw::event::CLOSE) {
            this->_running = false;
        } else {
            this->sendAction(event.second);
        }
    }
}

void client::update(void)
{
    this->_displayManager.update();
}

void client::draw(void)
{
    this->_displayManager.clear();

    for (const auto &item : this->_game.getLoadedObjects()) {
        this->_displayManager.useEntity(*item.second.getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
    }

    for (const auto &item : this->_game.getLoadedHuds()) {
        this->_displayManager.useEntity(*item.second.getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
    }

    this->_displayManager.draw();
}

void client::mainloop(void)
{
    this->_running = true;

    while (this->_running) {
        this->event();
        this->update();
        this->draw();
        this->_clock.tick(60);
    }

}
