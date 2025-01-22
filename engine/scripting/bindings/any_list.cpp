#include "engineScripting.hpp"
#include "engineCore.hpp"

static int any_list_create(lua_State *ctx) {
    std::vector<std::any> *temp = new std::vector<std::any>;

    luaL_setmetatable(ctx, "any_list");

	return (1);
}

static int any_list_delete(lua_State *ctx) {
    delete *reinterpret_cast<std::vector<std::any>**>(lua_touserdata(ctx, 1));

    return (0);
}

static int any_list_push_int(lua_State *ctx) {
    lua_Integer value = luaL_checkinteger(ctx, 2);
    auto list =  *reinterpret_cast<std::vector<std::any>**>(lua_touserdata(ctx, 1));

    list->push_back(std::any(value));
    return (0);
}

static int any_list_push_string(lua_State *ctx) {
    std::string value = luaL_checkstring(ctx, 2);
    auto list = *reinterpret_cast<std::vector<std::any>**>(lua_touserdata(ctx, 1));

    list->push_back(std::any(value));
    return (0);
}


void any_list_register(lua_State *ctx)
{
    lua_register(ctx, "any_list", any_list_create);

    luaL_newmetatable(ctx, "any_list");

    lua_pushcfunction(ctx, any_list_delete);
    lua_setfield(ctx, -2, "__gc");

    lua_pushvalue(ctx, -1);
    lua_setfield(ctx, -2, "__index");

    lua_pushcfunction(ctx, any_list_push_int);
    lua_setfield(ctx, -2, "push_int");

    lua_pushcfunction(ctx, any_list_push_string);
    lua_setfield(ctx, -2, "push_str");

    lua_pop(ctx, 1);
}
