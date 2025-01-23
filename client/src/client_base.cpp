#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>
#include <yaml-cpp/yaml.h>

using namespace engine;

client::client(const std::string &configPath)
  : _running(false)
{
    engine::ScriptTypeDefinitor<Game> *gameDefinitor = new engine::ScriptTypeDefinitor<Game>();
    engine::ScriptTypeDefinitor<grw::clock> *clockDefinitor = new engine::ScriptTypeDefinitor<grw::clock>();

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

    this->_orchestrator.registerGlobal(ScriptGlobalDefinition((engine::IScriptTypeDefinitor *)gameDefinitor, "game", "game", (void *)&(this->_game)));
    this->_orchestrator.registerGlobal(ScriptGlobalDefinition((engine::IScriptTypeDefinitor *)clockDefinitor, "clock", "clock", (void *)&(this->_clock)));
    this->_orchestrator.addBinding(clock_register);

    this->_game.addFactory(&this->_factory);

    if (gameData["entryScene"])
        this->_game.loadScene(gameData["entryScene"].as<std::string>());

    this->_orchestrator.fromGameObject(this->_game);

    this->_gameWindow = this->_displayManager.createWindow(800, 600);
}

client::~client()
{

}

void client::event(void)
{
    static bool up = false;
    static bool down = false;
    static bool left = false;
    static bool right = false;

    this->_running = !this->_displayManager.event();

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_Z) {
            up = true;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_S) {
            down = true;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_Q) {
            left = true;
    }
    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_D) {
            right = true;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_PRESSED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_PRESSED).key == grw::event::keys::K_SPACE) {
            this->_orchestrator.callFunctionAll("key_space");
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_Z) {
            up = false;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_Q) {
            left = false;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_S) {
            down = false;
    }

    if (this->_displayManager.hasEvent(this->_gameWindow, grw::event::KEY_RELEASED) &&
        this->_displayManager.getEvent(this->_gameWindow, grw::event::KEY_RELEASED).key == grw::event::keys::K_D) {
            right = false;
    }

    if (up)
        this->_orchestrator.callFunctionAll("key_z");
    if (down)
        this->_orchestrator.callFunctionAll("key_s");
    if (left)
        this->_orchestrator.callFunctionAll("key_q");
    if (right) {
        this->_orchestrator.callFunctionAll("key_d");
    }
}

void client::update(void)
{
    this->_orchestrator.callFunctionAll("update");

    this->_displayManager.update();
}

void client::draw(void)
{
    this->_displayManager.clear();

    for (const auto &item : this->_game.getLoadedObjects()) {
        this->_displayManager.useEntity(*item->getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
    }

    for (const auto &item : this->_game.getLoadedHuds()) {
        this->_displayManager.useEntity(*item->getEntity(), this->_game.getFactory()->getRegistry(), this->_gameWindow);
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
