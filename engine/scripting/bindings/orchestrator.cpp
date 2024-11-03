#include "engineScripting.hpp"

void orchestrator_register(lua_State *ctx)
{
    luaL_newmetatable(ctx, "orchestrator");

    lua_pushvalue(ctx, -1);
    lua_setfield(ctx, -2, "__index");

    lua_pop(ctx, 1);
}
