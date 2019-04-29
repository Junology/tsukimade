/*!
 * \file test_list.cpp
 * \brief A test with 2 dimensional vectors.
 * \details
 * The aim of the test is to verify the following functionality of the library:
 * - to push and check struct data;
 * - to receive Lua function.
 *
 * \copyright (c) 2019 Jun Yoshida.
 * The project is released under MIT License.
 * \date April 28, 2019: created
 */

#include <iostream>

extern "C" {
#include "vect2d.h"
}

#include "common.hpp"

//! Specialize `tsukimade::type_utility` for `Vector2D`.
template<>
struct tsukimade::type_utility<Vector2D>
{
    using target_type = Vector2D;

    static target_type check(lua_State* L, int i)
    {
        if(lua_type(L, i) != LUA_TTABLE)
            throw std::bad_cast{};

        Vector2D result;

        lua_getfield(L, i, "x");
        result.x = tsukimade::type_utility<double>::check(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, i, "y");
        result.y = tsukimade::type_utility<double>::check(L, -1);
        lua_pop(L, 1);

        return result;
    }

    static void push(lua_State* L, target_type val)
    {
        lua_newtable(L);
        lua_pushnumber(L, val.x);
        lua_setfield(L, -2, "x");

        lua_pushnumber(L, val.y);
        lua_setfield(L, -2, "y");
    }
};

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
        {NULL, NULL} };

    TestEnv env;

    env.register_library(testlib, "veclib");

    int error = env.dofile(argv[1]);

    if(error)
        std::cerr << "Error: " << env.get_errmsg() << std::endl;

    return env.get_result() ? 0 : -1;
}
