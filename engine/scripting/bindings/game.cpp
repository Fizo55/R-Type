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

static int change_scene(lua_State *ctx)
{
    std::string sceneName = luaL_checkstring(ctx, 2);
    engine::Game *mygame = (*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game")));
    auto gIndex = lua_getglobal(ctx, "orchestrator");
    std::cout << "global" << std::endl;
    engine::ScriptOrchestrator *tempOrchestrator = *reinterpret_cast<engine::ScriptOrchestrator**>(lua_touserdata(ctx, 3));
    std::cout << "tempo" << std::endl;
    mygame->loadScene(sceneName);
    std::cout << "loadsc" << std::endl;

    tempOrchestrator->fromGameObject(*mygame);
    std::cout << "from gameobj" << std::endl;
    lua_pop(ctx, 3);

    return 0;
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

    lua_pushcfunction(ctx, change_scene);
    lua_setfield(ctx, -2, "change_scene");

    lua_pop(ctx, 1);
}
