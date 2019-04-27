#include <iostream>

extern "C" {
#include "testlib.h"
}

#include "tsukimade.hpp"

//! Specialize `tsukimade::type_utility` for `Vector2D`.
template<>
struct tsukimade::type_utility<Vector2D>
{
    using target_type = Vector2D;

    static target_type to(lua_State* L, int i)
    {
        if(lua_type(L, i) != LUA_TTABLE)
            throw std::bad_cast{};

        Vector2D result;

        lua_getfield(L, i, "x");
        result.x = tsukimade::type_utility<double>::to(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, i, "y");
        result.y = tsukimade::type_utility<double>::to(L, -1);
        lua_pop(L, 1);

        return result;
    }

    static target_type push(lua_State* L, target_type val)
    {
        lua_newtable(L);
        lua_pushnumber(L, val.x);
        lua_setfield(L, -2, "x");

        lua_pushnumber(L, val.y);
        lua_setfield(L, -2, "y");
    }
};

#define REGISTER(f) {#f, tsukimade::fun_wrapper<decltype(f), f>}

static int g_result = 0;

void write_result(int res)
{
    g_result = res;
}

int main(int argc, char** argv)
{
    if(argc <= 1) {
        std::cerr << "Too few arguments." << std::endl;
        return -1;
    }

    const luaL_Reg testlib[] = {
        REGISTER(add_vecs),
        REGISTER(diff_vecs),
        REGISTER(scalar_prod),
        REGISTER(norm),
        REGISTER(gradient),
        REGISTER(write_result),
        {NULL, NULL} };

    lua_State* L = luaL_newstate();

    luaL_openlibs(L);
    luaL_newlib(L, testlib);
    lua_setglobal(L, "veclib");

    int error = luaL_dofile(L, argv[1]);

    if(error) {
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    lua_close(L);

    return (!error && g_result) ? 0 : -1;
}
