/*!
 * \file common.hpp
 * \brief 
 *   This file contains several macros and functions which are commonly used in the tests of the project.
 * \copyright (c) 2019 Jun Yoshida.
 * The project is released under MIT License.
 * \date April 29, 2019: created
 */

#ifndef TSUKIMADE_TEST_COMMON_HPP
#define TSUKIMADE_TEST_COMMON_HPP

#include "tsukimade.hpp"

#include <string>

// Macros
#define REGISTER(f) {#f, tsukimade::fun_wrapper<decltype(f), f>}


// Basic test environment
class TestEnv {
private:
    bool m_result; // ture means success, false means failure.
    lua_State* m_luaState;
    std::string m_errmsg;

public:
    TestEnv(bool init = true);
    ~TestEnv();

    lua_State* get_luastate()
    {
        return m_luaState;
    }

    void register_library(const luaL_Reg* lib, const std::string& name)
    {
        luaL_newlib(m_luaState, lib);
        lua_setglobal(m_luaState, name.c_str());
    }

    void write_result(bool result)
    {
        m_result = result;
    }

    bool get_result() const
    {
        return m_result;
    }

    const std::string& get_errmsg() const
    {
        return m_errmsg;
    }

    int dofile(const std::string& filename);

    int dostring(const std::string& luacode);
};

#endif
