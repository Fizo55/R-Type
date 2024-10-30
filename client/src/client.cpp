#include "gameClient.hpp"
#include "engineComponents.hpp"
#include "engineGraphic.hpp"
#include <filesystem>

using namespace engine;

client::client()
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

    this->_orchestrator.registerScript("test", "assets/scripts/test.lua");

    this->_orchestrator.registerGlobal(ScriptGlobalDefinition((engine::IScriptTypeDefinitor *)&gameDefinitor, "game", "game", (void *)&(this->_game)));

    this->_displayManager.registerAsset("sprite0", basePath + "assets/images/error.png");

    this->_game.registerObject(basePath + "assets/objects/test.yml");
    this->_game.registerScene(basePath + "assets/scenes/main_scene.yml");

    this->_game.addFactory(&this->_factory);

    this->_game.loadScene("scene0");

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
