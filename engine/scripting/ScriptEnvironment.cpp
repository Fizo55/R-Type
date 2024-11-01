#include "engineScripting.hpp"
#include "engineExceptions.hpp"

engine::ScriptEnvironment::ScriptEnvironment()
  : _ctx(luaL_newstate())
{
    if (!this->_ctx)
        return;
}

engine::ScriptEnvironment::~ScriptEnvironment()
{
    if (this->_ctx)
        lua_close(this->_ctx);
    this->_ctx = (lua_State *)0;
}

void engine::ScriptEnvironment::buildCoreLibrary()
{
    // game
    game_register(this->_ctx);

    // object
    object_register(this->_ctx);

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

lua_State *engine::ScriptEnvironment::getCtx(void) const
{
    return (this->_ctx);
}