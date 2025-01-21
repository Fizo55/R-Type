#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>
#include <yaml-cpp/yaml.h>

using namespace engine;

client::client(const std::string &configPath)
  : _running(false)
{
    engine::ScriptTypeDefinitor gameDefinitor = engine::ScriptTypeDefinitor<Game>();

    this->_factory.registerComponent<engine_components::Position>();
    this->_factory.registerComponent<engine_components::Size>();
    this->_factory.registerComponent<engine_components::Sprite>();
    this->_factory.registerComponent<engine_components::Script>();
    this->_factory.registerComponent<engine_components::Volatile>();

    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("position", engine_components::Position::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("size", engine_components::Size::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("sprite", engine_components::Sprite::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("script", engine_components::Script::buildComponent));
    this->_factory.registerBuildComponentRoute(ComponentBuildRoute("volatile", engine_components::Volatile::buildComponent));

#ifdef _WIN32
    std::string basePath = std::filesystem::current_path().string() + "\\";
#else
    std::string basePath = "";
#endif

    YAML::Node gameData = YAML::LoadFile(configPath);

    if (gameData["images"]) {
        for (YAML::const_iterator it = gameData["images"].begin(); it != gameData["images"].end(); ++it) {
            this->_displayManager.registerAsset(it->first.as<std::string>(), basePath + it->second.as<std::string>());
        }
    }

    if (gameData["scripts"]) {
        for (YAML::const_iterator it = gameData["scripts"].begin(); it != gameData["scripts"].end(); ++it) {
            this->_orchestrator.registerScript(it->first.as<std::string>(), basePath + it->second.as<std::string>());
        }
    }

    if (gameData["objects"]) {
        for (const auto &item : gameData["objects"]) {
            this->_game.registerObject(basePath + item.as<std::string>());
        }
    }

    if (gameData["scenes"]) {
        for (const auto &item : gameData["scenes"]) {
            this->_game.registerScene(basePath + item.as<std::string>());
        }
    }

    this->_orchestrator.registerGlobal(ScriptGlobalDefinition((engine::IScriptTypeDefinitor *)&gameDefinitor, "game", "game", (void *)&(this->_game)));

    this->_game.addFactory(&this->_factory);

    if (gameData["entryScene"])
        this->_game.loadScene(gameData["entryScene"].as<std::string>());

    this->_orchestrator.fromGameObject(this->_game);

    this->_gameWindow = this->_displayManager.createWindow();
}

client::~client()
{

}

void client::event(void)
{
    static bool isFlying = false;

    this->_running = !this->_displayManager.event();

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::NO_EVENT)) {
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_UP) {
        std::cout << "up" << std::endl;
        isFlying = true;
    }
        if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_DOWN) {
        std::cout << "down" << std::endl;
    }
        if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_LEFT) {
        std::cout << "left" << std::endl;
    }
        if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_RIGHT) {
        std::cout << "right" << std::endl;
    }
        if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_SPACE) {
        std::cout << "space" << std::endl;
        isFlying = true;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
    this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_SPACE) {
        std::cout << "released" << std::endl;
        isFlying = false;
    }
    if (isFlying) {
        this->_orchestrator.callFunctionAll("move_up");
    } else {
        this->_orchestrator.callFunctionAll("move_down");
    }
}

void client::update(void)
{
    this->_orchestrator.callFunctionAll("move_background");
    this->_orchestrator.callFunctionAll("nothing");


    this->_displayManager.update();
}

void client::draw(void)
{
    this->_displayManager.clear();

    for (const auto &item : this->_game.getLoadedObjects()) {
        this->_displayManager.useEntity(*item.getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
    }

    for (const auto &item : this->_game.getLoadedHuds()) {
        this->_displayManager.useEntity(*item.getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
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
