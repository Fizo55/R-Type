#include "engineScripting.hpp"
#include "engineCore.hpp"
#include "engineComponents.hpp"

static int object_create(lua_State *ctx)
{
    engine::Object *temp = new engine::Object();
    auto gIndex = lua_getglobal(ctx, "game");
    engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, gIndex));
    lua_pop(ctx, gIndex);

    temp->addBuildComponent("__def_volatility", "volatile");
    temp->buildEntity(*tempGame->getFactory());

	*reinterpret_cast<engine::Object**>(lua_newuserdata(ctx, sizeof(engine::Object*))) = temp;

    luaL_setmetatable(ctx, "object");

	return (1);
}

static int object_getname(lua_State *ctx)
{
    lua_pushstring(ctx, (*reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object")))->getName().c_str());

    return (1);
}

void object_register(lua_State *ctx)
{
    lua_register(ctx, "object", object_create);

    luaL_newmetatable(ctx, "object");

    // lua_pushcfunction(ctx, object_delete);
    // lua_setfield(ctx, -2, "__gc");

    lua_pushvalue(ctx, -1);
    lua_setfield(ctx, -2, "__index");

    lua_pushcfunction(ctx, object_getname);
    lua_setfield(ctx, -2, "get_name");

    lua_pop(ctx, 1);
}
