/*!
 * \file common.cpp
 * \brief Implementaion of utilities commonly used in tests.
 *
 * \copyright (c) 2019 Jun Yoshida.
 * The project is released under MIT License.
 * \date April 29, 2019: created
 */
#include <iostream>
#include "common.hpp"

// Lua function to write the test result in Lua.
int write_result(lua_State* L)
{
    auto env = static_cast<TestEnv*>(
        lua_touserdata(L, lua_upvalueindex(1)));

    env->write_result(lua_toboolean(L, -1));

    return 0;
}

// Implementaion of TestEnv class.
TestEnv::TestEnv(bool init)
    : m_result(init), m_luaState(luaL_newstate()), m_errmsg()
{
    luaL_openlibs(m_luaState);

    lua_pushlightuserdata(m_luaState, static_cast<void*>(this));
    lua_pushcclosure(m_luaState, ::write_result, 1);
    lua_setglobal(m_luaState, "write_result");
}

TestEnv::~TestEnv()
{
    lua_close(m_luaState);
}

int TestEnv::dofile(const std::string& filename)
{
    int error = luaL_dofile(m_luaState, filename.c_str());

    if(error) {
        m_errmsg.assign(lua_tostring(m_luaState, -1));
        lua_pop(m_luaState, 1);

        m_result = false;
    }

    return error;
}

int TestEnv::dostring(const std::string& luacode)
{
    int error = luaL_dostring(m_luaState, luacode.c_str());

    if(error) {
        m_errmsg.assign(lua_tostring(m_luaState, -1));
        lua_pop(m_luaState, 1);

        m_result = false;
    }

    return error;
}
