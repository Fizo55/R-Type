#include "engineScripting.hpp"

static int game_delete(lua_State *ctx)
{
    delete *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 1));
    return (0);
}

static int game_test(lua_State *ctx)
{
    lua_pushnumber(ctx, (lua_Number)(std::size_t)(*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game"))));
    return (0);
}

void game_register(lua_State *ctx)
{
    luaL_newmetatable(ctx, "game");

    lua_pushcfunction(ctx, game_delete);
    lua_setfield(ctx, -2, "__gc");

    lua_pushvalue(ctx, -1);
    lua_setfield(ctx, -2, "__index");

    lua_pushcfunction(ctx, game_test);
    lua_setfield(ctx, -2, "test");

    lua_pop(ctx, 1);
}
