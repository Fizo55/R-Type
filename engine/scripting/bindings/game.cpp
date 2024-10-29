#include "engineScripting.hpp"

static int game_delete(lua_State *ctx)
{
    delete *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 1));
    return (0);
}

static int game_test(lua_State *ctx)
{
    std::cout << "the game is real " << (std::size_t)(void *)(*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game"))) << std::endl;

    return (0);
}

void game_register(lua_State *ctx)
{
    // lua_register(ctx, "game", game_new); NO CONSTRUCTOR FOR GAME, POR FAVOR

    luaL_newmetatable(ctx, "game");

    // lua_pushcfunction(ctx, game_delete);
    // lua_setfield(ctx, -2, "__gc"); NO DESTRUCTOR FOR GAME, POR FAVOR

    lua_pushvalue(ctx, -1);
    lua_setfield(ctx, -2, "__index");

    lua_pushcfunction(ctx, game_test);
    lua_setfield(ctx, -2, "test");

    lua_pop(ctx, 1);
}
