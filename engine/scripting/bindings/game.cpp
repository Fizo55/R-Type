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
    engine::ScriptOrchestrator *tempOrchestrator = *reinterpret_cast<engine::ScriptOrchestrator**>(lua_touserdata(ctx, 3));
    mygame->loadScene(sceneName);

    tempOrchestrator->fromGameObject(*mygame);
    lua_pop(ctx, 3);

    return 0;
}

static int add_object(lua_State *ctx)
{
    auto object = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 2, "object"));
    engine::Game *mygame = (*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game")));
    auto gIndex = lua_getglobal(ctx, "orchestrator");
    engine::ScriptOrchestrator *tempOrchestrator = *reinterpret_cast<engine::ScriptOrchestrator**>(lua_touserdata(ctx, 3));

    mygame->loadObject(object);
    tempOrchestrator->fromObject(mygame->getFactory(), object);

    lua_pop(ctx, 3);

    return 0;
}

static int remove_object(lua_State *ctx)
{
    auto object = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 2, "object"));
    engine::Game *mygame = (*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game")));
    auto gIndex = lua_getglobal(ctx, "orchestrator");
    engine::ScriptOrchestrator *tempOrchestrator = *reinterpret_cast<engine::ScriptOrchestrator**>(lua_touserdata(ctx, 3));

    mygame->unloadObject(object);
    tempOrchestrator->removeScript((long)object);

    delete object;

    lua_pop(ctx, 3);

    return 0;
}

static int get_object(lua_State *ctx)
{
    lua_Integer index = luaL_checkinteger(ctx, 2);
    engine::Game *mygame = (*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game")));
    const std::vector<engine::Object *> objects = mygame->getLoadedObjects();

    if (index >= objects.size())
        lua_pushnil(ctx);
    else
        *reinterpret_cast<engine::Object**>(lua_newuserdata(ctx, sizeof(engine::Object*))) = objects[index];

    luaL_setmetatable(ctx, "object");

    return 1;
}

static int write_db_int(lua_State *ctx)
{
    lua_Integer index = luaL_checkinteger(ctx, 2);
    lua_Integer value = luaL_checkinteger(ctx, 3);
    engine::Game *mygame = (*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game")));

    mygame->writeDBInt(index, value);

    return 0;
}

static int read_db_int(lua_State *ctx)
{
    lua_Integer index = luaL_checkinteger(ctx, 2);
    engine::Game *mygame = (*reinterpret_cast<engine::Game**>(luaL_checkudata(ctx, 1, "game")));

    lua_pushinteger(ctx, mygame->readDBInt(index));

    return 1;
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

    lua_pushcfunction(ctx, add_object);
    lua_setfield(ctx, -2, "add_object");

    lua_pushcfunction(ctx, remove_object);
    lua_setfield(ctx, -2, "remove_object");

    lua_pushcfunction(ctx, get_object);
    lua_setfield(ctx, -2, "get_object");

    lua_pushcfunction(ctx, read_db_int);
    lua_setfield(ctx, -2, "read_int");

    lua_pushcfunction(ctx, write_db_int);
    lua_setfield(ctx, -2, "write_int");

    lua_pop(ctx, 1);
}
