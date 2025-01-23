#include "engineScripting.hpp"
#include "engineComponents.hpp"

engine::ScriptOrchestrator::ScriptOrchestrator()
{

}

engine::ScriptOrchestrator::~ScriptOrchestrator()
{
    for (auto item : this->_scripts) {
        item->stopUsing();
        delete item;
    }

    this->_scripts.clear();
}

void engine::ScriptOrchestrator::fromObject(engine::EntityFactory *factory, Object *object)
{
    auto &script = factory->getRegistry().get_component<engine_components::Script>(*object->getEntity());
    std::vector<engine::ScriptGlobalDefinition> temp;
    engine::ScriptTypeDefinitor tempDefinitor = engine::ScriptTypeDefinitor<Object>();
    engine::ScriptTypeDefinitor tempDefinitorOrchestrator = engine::ScriptTypeDefinitor<engine::ScriptOrchestrator>();

    if (!script)
        return;

    temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitorOrchestrator), "orchestrator", "orchestrator", (void *)(this)));
    temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitor), "object", "self", (void *)(object)));

    this->buildScript(script->script, temp);
}

void engine::ScriptOrchestrator::fromGameObject(Game &game)
{
    for (const auto &temp : this->_scripts) {
        temp->stopUsing();
        delete temp;
    }
    this->_scripts.clear();

    for (const auto &obj : game.getLoadedObjects()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj->getEntity());
        std::vector<engine::ScriptGlobalDefinition> temp;
        engine::ScriptTypeDefinitor tempDefinitor = engine::ScriptTypeDefinitor<Object>();
        engine::ScriptTypeDefinitor tempDefinitorOrchestrator = engine::ScriptTypeDefinitor<engine::ScriptOrchestrator>();

        if (!script)
            continue;

        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitorOrchestrator), "orchestrator", "orchestrator", (void *)(this)));
        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitor), "object", "self", (void *)(obj)));

        this->buildScript(script->script, temp);
    }

    for (const auto &obj : game.getLoadedHuds()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj->getEntity());
        std::vector<engine::ScriptGlobalDefinition> temp;
        engine::ScriptTypeDefinitor tempDefinitor = engine::ScriptTypeDefinitor<Object>();
                engine::ScriptTypeDefinitor tempDefinitorOrchestrator = engine::ScriptTypeDefinitor<engine::ScriptOrchestrator>();

        if (!script)
            continue;

        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitorOrchestrator), "orchestrator", "orchestrator", (void *)(this)));
        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitor), "object", "self", (void *)(obj)));

        this->buildScript(script->script, temp);
    }
}

void engine::ScriptOrchestrator::callFunctionAll(const std::string &name)
{
    for (auto it : this->_scripts) {
        it->callFunction(name);
    }
}

void engine::ScriptOrchestrator::registerScript(const std::string &name, const std::string &path)
{
    this->_registeredScripts[name] = path;
}

void engine::ScriptOrchestrator::buildScript(const std::string &name, const std::vector<engine::ScriptGlobalDefinition> &extraDefs)
{
    ScriptEnvironment *newScript = new ScriptEnvironment();

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

    this->_scripts.push_back(newScript);
}

void engine::ScriptOrchestrator::addBinding(std::function<void(lua_State *)> binding)
{
    this->_bindings.push_back(binding);
}