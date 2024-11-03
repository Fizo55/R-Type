#include "engineScripting.hpp"
#include "engineComponents.hpp"

engine::ScriptOrchestrator::ScriptOrchestrator()
{

}

void engine::ScriptOrchestrator::runLuaFunctions(const std::string &scriptName, const std::string &functionName, const std::vector<std::string> &args)
{

    std::cout << "step1: " << scriptName << std::endl;
    auto currentScript = this->_scripts.find(scriptName);
    if (currentScript == this->_scripts.end()) {
        std::cerr << "Script " << scriptName << " not found." << std::endl;
        return;
    }
    std::cout << "step2" << std::endl;

    auto &scriptEnv = currentScript->second;
    lua_State *ctx = scriptEnv.getCtx();

    lua_getglobal(ctx, functionName.c_str());
    if (!lua_isfunction(ctx, -1)) {
        std::cerr << "Function " << functionName << " not found." << std::endl;
        lua_pop(ctx, 1);
        return;
    }

    std::cout << "step3" << std::endl;
    for (const auto &arg : args) {
        lua_pushstring(ctx, arg.c_str());
    }

    std::cout << "step4" << std::endl;
    if (lua_pcall(ctx, args.size(), 0, 0) != LUA_OK) {
        std::cerr << "Error calling function " << lua_tostring(ctx, -1) << std::endl;
        lua_pop(ctx, 1);
    }
}

void engine::ScriptOrchestrator::fromGameObject(Game &game)
{
    for (const auto &obj : game.getLoadedObjects()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj.getEntity());
        std::vector<engine::ScriptGlobalDefinition> temp;
        engine::ScriptTypeDefinitor tempDefinitor = engine::ScriptTypeDefinitor<Object>();

        if (!script)
            continue;

        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitor), "object", "self", (void *)(&obj)));

        this->buildScript(script->script, temp);
    }

    for (const auto &obj : game.getLoadedHuds()) {
        auto &script = game.getFactory()->getRegistry().get_component<engine_components::Script>(*obj.getEntity());
        std::vector<engine::ScriptGlobalDefinition> temp;
        engine::ScriptTypeDefinitor tempDefinitor = engine::ScriptTypeDefinitor<Object>();

        if (!script)
            continue;

        temp.push_back(engine::ScriptGlobalDefinition((IScriptTypeDefinitor *)(&tempDefinitor), "object", "self", (void *)(&obj)));


        this->buildScript(script->script, temp);
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