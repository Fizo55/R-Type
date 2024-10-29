#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>

using namespace engine;

client::client()
  : _running(false)
{
    this->_factory.registerComponent<engine_components::Position>();
    this->_factory.registerComponent<engine_components::Sprite>();
    this->_factory.registerComponent<engine_components::Script>();

    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("position", engine_components::Position::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("sprite", engine_components::Sprite::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("script", engine_components::Script::buildComponent));

#ifdef _WIN32
    std::string basePath = std::filesystem::current_path().string() + "\\";
#else
    std::string basePath = "";
#endif

    this->_orchestrator.registerScript("test", "assets/scripts/test.lua");

    this->_displayManager.registerAsset("sprite0", basePath + "assets/images/error.png");

    this->_game.registerObject(basePath + "assets/objects/test.yml");
    this->_game.registerScene(basePath + "assets/scenes/main_scene.yml");

    this->_game.addFactory(&this->_factory);

    this->_game.loadScene("scene0");

    this->_orchestrator.fromGameObject(this->_game);

    this->_gameWindow = this->_displayManager.createWindow();
    
    try
    {
        grw::init();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
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
    auto playerInputMessage = std::make_shared<PlayerInputMessage>();

    switch(event.type) {
        case grw::event::UP:
            playerInputMessage->inputFlags |= 0x01;
            break;
        case grw::event::DOWN:
            playerInputMessage->inputFlags |= 0x02;
            break;
        case grw::event::LEFT:
            playerInputMessage->inputFlags |= 0x04;
            break;
        case grw::event::RIGHT:
            playerInputMessage->inputFlags |= 0x08;
            break;
        case grw::event::SHOOT:
            playerInputMessage->inputFlags |= 0x10;
            break;
        default:
            break;
    }

    std::vector<uint8_t> outData;
    playerInputMessage->serialize(outData);

    network_->send(outData, _host, _port);
}

void client::event(void)
{
    this->_running = !this->_displayManager.event();
    try
    {
        _events = this->_displayManager.getEvents(this->_gameWindow);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    
    for (auto &event : _events) {
        if (event.second.type == grw::event::QUIT || event.second.type == grw::event::CLOSE) {
            this->_running = false;
        } else if (event.second.type != grw::event::NO_EVENT) {
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
