#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>
#include <yaml.h>

using namespace engine;

client::client(const std::string &configPath)
  : _running(false)
{
    engine::ScriptTypeDefinitor gameDefinitor = engine::ScriptTypeDefinitor<Game>();

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
    this->_running = !this->_displayManager.event();
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
