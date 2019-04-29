/*!
 * \file test_list.cpp
 * \brief A test with elementary operations on simple lists.
 * \details
 * The aim of the test is to verify the following functionality of the library:
 * - to pass pointers to data structures to Lua;
 * - to manipulate passed data structures in Lua;
 * - to receive functions as arguments;
 * - received functions may admit arguments of function types;
 * - to push and check enum data.
 *
 * \copyright (c) 2019 Jun Yoshida.
 * The project is released under MIT License.
 * \date April 29, 2019: created
 */

#include <iostream>

extern "C" {
#include "list.h"
}

#include "common.hpp"

simple_flist* create_empty_list()
{
    return nullptr;
}

int main(int argc, char** argv)
{
    if(argc <= 1) {
        std::cerr << "Too few arguments." << std::endl;
        return -1;
    }

    const luaL_Reg testlib[] = {
        REGISTER(create_empty_list),
        REGISTER(get_last),
        REGISTER(get_length),
        REGISTER(get_value),
        REGISTER(push_front),
        REGISTER(pop_front),
        REGISTER(push_back),
        REGISTER(pop_back),
        REGISTER(destroy),
        REGISTER(filter),
        REGISTER(compare_canonical),
        REGISTER(compare_opposite),
        REGISTER(bubble_sort),
        REGISTER(get_median),
        {NULL, NULL} };

    TestEnv env;

    env.register_library(testlib, "list");

    /* Basic test for list library */ {
        simple_flist* testlist = nullptr;

        testlist = push_back(testlist, 3);
        testlist = push_back(testlist, 1);
        testlist = push_front(testlist, 4);
        testlist = push_back(testlist, 1);
        testlist = push_front(testlist, 5);
        testlist = push_front(testlist, 9);
        testlist = push_back(testlist, 2);
        testlist = push_back(testlist, 6);
        testlist = push_front(testlist, 5);
        testlist = push_front(testlist, 3);
        testlist = push_back(testlist, 5);
        testlist = push_front(testlist, 8);
        testlist = push_back(testlist, 9);
        testlist = push_back(testlist, 7);
        testlist = push_front(testlist, 9);

    // 9, 8, 3, 5, 9, 5, 4, 3, 1, 1, 2, 6, 5, 9, 7
    // sorted -> 1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 7, 8, 9, 9, 9
        try {
            if(get_length(testlist) != 15 && get_value(testlist, 3) != 5)
                throw __LINE__;

            testlist = push_front(testlist, 57);
            if(get_length(testlist) != 16 && get_value(testlist, 0) != 57)
                throw __LINE__;

            testlist = pop_back(testlist);
            if(get_length(testlist) != 15)
                throw __LINE__;

            testlist = push_back(testlist, 128);
            testlist = bubble_sort(testlist, compare_opposite);
            if(get_value(testlist, 0) != 128)
                throw __LINE__;

            int med = get_median(testlist, compare_canonical, bubble_sort);
            if(med != 5)
                throw __LINE__;
        }
        catch (int line) {
            std::cerr << "Error in basic test at line " << line << std::endl;
            env.write_result(false);
        }

        destroy(testlist);
    }

    int error = env.dofile(argv[1]);

    if(error)
        std::cerr << "Error: " << env.get_errmsg() << std::endl;

    return env.get_result() ? 0 : -1;
}
