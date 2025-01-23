#include "engineScripting.hpp"
#include "engineCore.hpp"
#include "engineComponents.hpp"

static int object_create(lua_State *ctx)
{
    engine::Object *temp = new engine::Object();
    auto gIndex = lua_getglobal(ctx, "game");
    engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 1));

    temp->addBuildComponent("__def_volatility", "volatile");
    temp->buildEntity(tempGame->getFactory());

    lua_pop(ctx, 1);

	*reinterpret_cast<engine::Object**>(lua_newuserdata(ctx, sizeof(engine::Object*))) = temp;

    luaL_setmetatable(ctx, "object");

	return (1);
}

static int object_add_component(lua_State *ctx)
{
    std::string name = luaL_checkstring(ctx, 2);
    std::string component_name = luaL_checkstring(ctx, 3);

    //auto gIndex = lua_getglobal(ctx, "game");
    auto self = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object"));
    //engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 4));

    self->addBuildComponent(name, component_name);

    //lua_pop(ctx, 4);
    return (0);
}

static int object_add_parameter(lua_State *ctx)
{
    std::string name = luaL_checkstring(ctx, 2);
    std::vector<std::any> *parameter = *reinterpret_cast<std::vector<std::any> **>(luaL_checkudata(ctx, 3, "any_list"));

    //auto gIndex = lua_getglobal(ctx, "game");
    auto self = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object"));
    //engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 4));

    self->addBuildParameter(name, *parameter);

    //lua_pop(ctx, 4);
    return (0);
}

static int object_compile(lua_State *ctx)
{
    auto gIndex = lua_getglobal(ctx, "game");
    auto self = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object"));
    engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 2));

    self->buildEntity(tempGame->getFactory());

    lua_pop(ctx, 2);
    return (0);
}

static int object_move(lua_State *ctx)
{
    lua_Number x = luaL_checknumber(ctx, 2);
    lua_Number y = luaL_checknumber(ctx, 3);
    auto gIndex = lua_getglobal(ctx, "game");
    auto self = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object"));
    engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 4));

    std::optional<engine_components::Position> &tempPos = (tempGame->getFactory())->getRegistry().get_component<engine_components::Position>(*self->getEntity());

    if (!tempPos)
        return 0;

    tempPos->coordinates.x = (int)x;
    tempPos->coordinates.y = (int)y;

    lua_pop(ctx, 4);
    return (0);
}

static int object_getpos(lua_State *ctx)
{
    std::string axis = luaL_checkstring(ctx, 2);

    auto gIndex = lua_getglobal(ctx, "game");
    auto self = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object"));
    engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 3));

    std::optional<engine_components::Position> &tempPos = (tempGame->getFactory())->getRegistry().get_component<engine_components::Position>(*self->getEntity());

    if (!tempPos)
        return 0;

    if (axis == "x") {
        lua_pushinteger(ctx, tempPos->coordinates.x);
        return 1;
    }
    if (axis == "y") {
        lua_pushinteger(ctx, tempPos->coordinates.y);
        return 1;
    }

    lua_pop(ctx, 3);
    return (0);
}

static int object_getsize(lua_State *ctx)
{
    std::string axis = luaL_checkstring(ctx, 2);

    auto gIndex = lua_getglobal(ctx, "game");
    auto self = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object"));
    engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 3));

    std::optional<engine_components::Size> &tempSize = (tempGame->getFactory())->getRegistry().get_component<engine_components::Size>(*self->getEntity());

    if (!tempSize)
        return 0;

    if (axis == "x") {
        lua_pushinteger(ctx, tempSize->coordinates.x);
        return 1;
    }
    if (axis == "y") {
        lua_pushinteger(ctx, tempSize->coordinates.y);
        return 1;
    }

    lua_pop(ctx, 3);
    return (0);
}

static int object_getname(lua_State *ctx)
{
    lua_pushstring(ctx, (*reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object")))->getName().c_str());

    return (1);
}

static int object_setname(lua_State *ctx)
{
    std::string name = luaL_checkstring(ctx, 2);
    auto self = *reinterpret_cast<engine::Object**>(luaL_checkudata(ctx, 1, "object"));

    self->setName(name);

    return (0);
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

    lua_pushcfunction(ctx, object_getpos);
    lua_setfield(ctx, -2, "get_pos");

    lua_pushcfunction(ctx, object_getsize);
    lua_setfield(ctx, -2, "get_size");

    lua_pushcfunction(ctx, object_move);
    lua_setfield(ctx, -2, "move");

    lua_pushcfunction(ctx, object_setname);
    lua_setfield(ctx, -2, "set_name");

    lua_pushcfunction(ctx, object_add_component);
    lua_setfield(ctx, -2, "add_component");

    lua_pushcfunction(ctx, object_add_parameter);
    lua_setfield(ctx, -2, "add_parameter");

    lua_pushcfunction(ctx, object_compile);
    lua_setfield(ctx, -2, "compile");

    lua_pop(ctx, 1);
}
