#include "engineScripting.hpp"
#include "engineGraphic.hpp"

static int display_manager_display_text(lua_State *ctx)
{
    std::string text = luaL_checkstring(ctx, 2);
    std::string font_name = luaL_checkstring(ctx, 3);
    lua_Integer size = luaL_checkinteger(ctx, 4);
    lua_Integer x = luaL_checkinteger(ctx, 5);
    lua_Integer y = luaL_checkinteger(ctx, 6);
    lua_Integer size_x = luaL_checkinteger(ctx, 7);
    lua_Integer size_y = luaL_checkinteger(ctx, 8);

    auto gIndex = lua_getglobal(ctx, "game");
    engine::Game *tempGame = *reinterpret_cast<engine::Game**>(lua_touserdata(ctx, 9));

    auto self = *reinterpret_cast<engine::displayManager**>(luaL_checkudata(ctx, 1, "display_manager"));

    self->useText(text, font_name, size, engine_math::vector2<int>(x, y), engine_math::vector2<int>(size_x, size_y), tempGame->readDBInt(0xF1));

    lua_pop(ctx, 9);

    return (0);
}


void display_manager_register(lua_State *ctx)
{
    luaL_newmetatable(ctx, "display_manager");

    lua_pushvalue(ctx, -1);
    lua_setfield(ctx, -2, "__index");

    lua_pushcfunction(ctx, display_manager_display_text);
    lua_setfield(ctx, -2, "display_text");

    lua_pop(ctx, 1);
}
