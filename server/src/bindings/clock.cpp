#include "engineScripting.hpp"
#include "gameServer.hpp"

static int clock_last_tick(lua_State *ctx)
{
    auto gIndex = lua_getglobal(ctx, "game");
    auto self = *reinterpret_cast<Clock**>(luaL_checkudata(ctx, 1, "clock"));

    lua_pushinteger(ctx, self->lastDT());
    return 1;
}

void clock_register(lua_State *ctx)
{
    luaL_newmetatable(ctx, "clock");

    lua_pushvalue(ctx, -1);
    lua_setfield(ctx, -2, "__index");

    lua_pushcfunction(ctx, clock_last_tick);
    lua_setfield(ctx, -2, "get_last_tick");

    lua_pop(ctx, 1);
}
