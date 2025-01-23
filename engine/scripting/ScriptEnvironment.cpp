#include "engineScripting.hpp"
#include "engineExceptions.hpp"

engine::ScriptEnvironment::ScriptEnvironment(std::size_t ep)
  : ep(ep), _ctx(luaL_newstate())
{
    this->doStopUsing = false;

    if (!this->_ctx)
        return;
}

engine::ScriptEnvironment::~ScriptEnvironment()
{
    this->doStopUsing = true;

    if (this->_ctx)
        lua_close(this->_ctx);

    this->ep = 0x00;
    this->_ctx = (lua_State *)0;
}

void engine::ScriptEnvironment::stopUsing(void)
{
    this->doStopUsing = true;
}

void engine::ScriptEnvironment::buildCoreLibrary()
{
    // game
    game_register(this->_ctx);

    any_list_register(this->_ctx);

    // object
    object_register(this->_ctx);

    orchestrator_register(this->_ctx);

    // std
    luaL_openlibs(this->_ctx);
}

void engine::ScriptEnvironment::loadScript(const std::string &path)
{
    if (luaL_loadfile(this->_ctx, path.c_str()))
        throw engine::loadError(path + ": failed to load lua file: " + lua_tostring(this->_ctx, -1));
    if (lua_pcall(this->_ctx, 0, 0, 0))
        throw engine::loadError(path + ": failed to load lua file: " + lua_tostring(this->_ctx, -1));
}

void engine::ScriptEnvironment::callFunction(const std::string &name)
{
    if (this->_ctx && !this->doStopUsing) {
        lua_getglobal(this->_ctx, name.c_str());

        if (!lua_isnil(_ctx, -1) && lua_isfunction(_ctx, -1)) {
            lua_pcall(_ctx, 0, 1, 0);
        }
        lua_pop(_ctx, -1);
    }
}

void engine::ScriptEnvironment::registerGlobalObject(const ScriptGlobalDefinition &scriptDef)
{
    scriptDef.type->magicSet(lua_newuserdata(this->_ctx, scriptDef.type->magicSize()), scriptDef.data);
    luaL_setmetatable(this->_ctx, scriptDef.luaTable.c_str());
    lua_setglobal(this->_ctx, scriptDef.name.c_str());
}

lua_State *engine::ScriptEnvironment::getCtx(void) const
{
    return (this->_ctx);
}