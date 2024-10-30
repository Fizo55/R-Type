#include "engineScripting.hpp"
#include "engineComponents.hpp"

engine::ScriptOrchestrator::ScriptOrchestrator()
{

}

void engine::ScriptOrchestrator::fromGameObject(Game &game)
{
    for (const auto &obj : game.getLoadedObjects()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj.second.getEntity());

        if (!script)
            continue;

        this->buildScript(script->script);
    }

    for (const auto &obj : game.getLoadedHuds()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj.second.getEntity());

        if (!script)
            continue;

        this->buildScript(script->script);
    }
}

void engine::ScriptOrchestrator::registerScript(const std::string &name, const std::string &path)
{
    this->_registeredScripts[name] = path;
}

void engine::ScriptOrchestrator::buildScript(const std::string &name)
{
    std::shared_ptr<ScriptEnvironment> newScript = std::make_shared<ScriptEnvironment>();

    newScript->buildCoreLibrary();

    for (const auto &item : this->_bindings) {
        item(newScript->getCtx());
    }

    for (const auto &item : this->_luaGlobals) {
        newScript->registerGlobalObject(item);
    }

    newScript->loadScript(this->_registeredScripts[name]);

    this->_scripts[name] = newScript;
}

void engine::ScriptOrchestrator::addBinding(std::function<void(lua_State *)> binding)
{
    this->_bindings.push_back(binding);
}