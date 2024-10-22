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
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, std::to_string(port));
    _socket = boost::asio::ip::tcp::socket(io_context);
    boost::asio::connect(_socket, endpoints);
}


/**
 * @brief Send an action to the server
 * 
 * @param event The event to send
 */
void client::sendAction(grw::event &event)
{
    std::vector<uint8_t> outData;
    gameStateMessage->serialize(outData);

    for (const auto& [address, player] : players_) {
        std::string ip;
        uint16_t port;
        size_t separatorPos = address.find(':');
        if (separatorPos != std::string::npos) {
            ip = address.substr(0, separatorPos);
            port = static_cast<uint16_t>(std::stoi(address.substr(separatorPos + 1)));
            network_->send(outData, ip, port);
        }
    }
}

void client::event(void)
{
    this->_running = !this->_displayManager.event();
    _events = this->_displayManager.getEvents(this->_gameWindow);
    for (const auto &event : _events) {
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
