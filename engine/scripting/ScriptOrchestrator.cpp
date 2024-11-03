#include "engineScripting.hpp"
#include "engineComponents.hpp"

engine::ScriptOrchestrator::ScriptOrchestrator()
{

}

void engine::ScriptOrchestrator::fromGameObject(Game &game)
{
    this->_scripts.clear();

    for (const auto &obj : game.getLoadedObjects()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj.getEntity());
        std::vector<engine::ScriptGlobalDefinition> temp;
        engine::ScriptTypeDefinitor tempDefinitor = engine::ScriptTypeDefinitor<Object>();
        engine::ScriptTypeDefinitor tempDefinitorOrchestrator = engine::ScriptTypeDefinitor<engine::ScriptOrchestrator>();

        if (!script)
            continue;

        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitorOrchestrator), "orchestrator", "orchestrator", (void *)(this)));
        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitor), "object", "self", (void *)(&obj)));

        this->buildScript(script->script, temp);
    }

    for (const auto &obj : game.getLoadedHuds()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj.getEntity());
        std::vector<engine::ScriptGlobalDefinition> temp;
        engine::ScriptTypeDefinitor tempDefinitor = engine::ScriptTypeDefinitor<Object>();
                engine::ScriptTypeDefinitor tempDefinitorOrchestrator = engine::ScriptTypeDefinitor<engine::ScriptOrchestrator>();

        if (!script)
            continue;

        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitorOrchestrator), "orchestrator", "orchestrator", (void *)(this)));
        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitor), "object", "self", (void *)(&obj)));

        this->buildScript(script->script, temp);
    }
}

void engine::ScriptOrchestrator::callFunctionAll(const std::string &name)
{
    for (auto it = this->_scripts.begin(); it != this->_scripts.end(); ++it) {
        it->second->callFunction(name);
    }
}

void engine::ScriptOrchestrator::registerScript(const std::string &name, const std::string &path)
{
    this->_registeredScripts[name] = path;
}

void engine::ScriptOrchestrator::buildScript(const std::string &name, const std::vector<engine::ScriptGlobalDefinition> &extraDefs)
{
    std::shared_ptr<ScriptEnvironment> newScript = std::make_shared<ScriptEnvironment>();

    newScript->buildCoreLibrary();

    for (const auto &item : this->_bindings) {
        item(newScript->getCtx());
    }

    for (const auto &item : this->_luaGlobals) {
        newScript->registerGlobalObject(item);
    }

    for (const auto &item : extraDefs) {
        newScript->registerGlobalObject(item);
    }

    newScript->loadScript(this->_registeredScripts[name]);

    this->_scripts[name] = newScript;
}

void engine::ScriptOrchestrator::addBinding(std::function<void(lua_State *)> binding)
{
    this->_bindings.push_back(binding);
}