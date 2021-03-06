/*!
 * \file tsukimade.hpp
 * \author Jun Yoshida
 * \brief
 *     The main header of the library "tsukimade"
 *
 * \copyright (c) 2019 Jun Yoshida.
 * The project is released under MIT License.
 * \date April 27, 2019: created
 */

#ifndef TSUKIMADE_MAIN_HEADER
#define TSUKIMADE_MAIN_HEADER

#include <tuple>
#include <functional>
#include <utility>
#include <typeinfo>

#include <lua.hpp>

#include "tsukimade_utils.hpp"

namespace tsukimade {

/*!
 * Assigning Lua types to C fundamental types.
 */
template<class T>
struct type_dictionary;

template<>
struct type_dictionary<int> { using type = lua_Integer; };

template<>
struct type_dictionary<long int> { using type = lua_Integer; };

template<>
struct type_dictionary<long long int> { using type = lua_Integer; };

template<>
struct type_dictionary<short int> { using type = lua_Integer; };

template<>
struct type_dictionary<unsigned int> { using type = lua_Unsigned; };

template<>
struct type_dictionary<unsigned long int> { using type = lua_Unsigned; };

template<>
struct type_dictionary<unsigned long long int> { using type = lua_Unsigned; };

template<>
struct type_dictionary<unsigned short int> { using type = lua_Unsigned; };

template<>
struct type_dictionary<float> { using type = lua_Number; };

template<>
struct type_dictionary<double> { using type = lua_Number; };

template<>
struct type_dictionary<long double> { using type = lua_Number; };


/*!
 * a function with arguments supllied from Lua stack.
 * \tparam F The type of functions, usually of the form R(*)(Args...)
 * \tparam D The arbitrary type decorating the function.
 * \tparam L Lua state with which the function is executed.
 * \param f The executed function.
 * \return The number of results; i.e. 0 if f is void(...) and 1 otherwise.
 */
template<class F, class D=void>
int apply_stack(lua_State* L, F f);

/*!
 * Generating prototypes of functions to communicate with Lua stacks.
 * \tparam T the target type.
 * \tparam Fcheck the function to read from the stack.
 * \tparam Fpush the function to write to the stack.
 */
template<
    class T,
    typename type_dictionary<T>::type(*Fcheck)(lua_State*,int),
    void(*Fpush)(lua_State*,typename type_dictionary<T>::type)>
struct type_utility_generic
{
    using target_type = T;

    static target_type check(lua_State* L, int i)
    {
        return static_cast<target_type>(Fcheck(L, i));
    }

    static void push(lua_State* L, target_type val)
    {
        Fpush(L, static_cast<typename type_dictionary<T>::type>(val));
    }
};

/*!
 * A set of functions to communicate with Lua stacks.
 * One can specialize this class to write functions to interact with Lua stacks.
 * \tparam T The target type.
 */
template<class T>
struct type_utility {
    using target_type = T;

    static const char* tname()
    {
        return typeid(utils::annihilate_cv_t<T>).name();
    }

    static target_type check(lua_State* L, int i) {
        target_type* retpp
            = static_cast<target_type*>(luaL_checkudata(L, i, tname()));

        return (*retpp);
    }

    static void push(lua_State* L, target_type data) {
        target_type* p_luadata
            = static_cast<target_type*>(lua_newuserdata(L, sizeof(target_type)));
        luaL_newmetatable(L, tname());
        lua_setmetatable(L, -2);

        if(p_luadata)
            (*p_luadata) = data;
    }
};

template<>
struct type_utility<int>
    : type_utility_generic<int, luaL_checkinteger, lua_pushinteger>
{};

template<>
struct type_utility<long int>
    : type_utility_generic<long int, luaL_checkinteger, lua_pushinteger>
{};

template<>
struct type_utility<long long int>
    : type_utility_generic<long long int, luaL_checkinteger, lua_pushinteger>
{};

template<>
struct type_utility<short int>
    : type_utility_generic<short int, luaL_checkinteger, lua_pushinteger>
{};

template<>
struct type_utility<unsigned int>
    : type_utility_generic<unsigned int, luaL_checkunsigned, lua_pushunsigned>
{};

template<>
struct type_utility<unsigned long int>
    : type_utility_generic<unsigned long int, luaL_checkunsigned, lua_pushunsigned>
{};

template<>
struct type_utility<unsigned long long int>
    : type_utility_generic<unsigned long long int, luaL_checkunsigned, lua_pushunsigned>
{};

template<>
struct type_utility<unsigned short int>
    : type_utility_generic<unsigned short int, luaL_checkunsigned, lua_pushunsigned>
{};

template<>
struct type_utility<float>
    : type_utility_generic<float, luaL_checknumber, lua_pushnumber>
{};

template<>
struct type_utility<double>
    : type_utility_generic<double, luaL_checknumber, lua_pushnumber>
{};

template<>
struct type_utility<long double>
    : type_utility_generic<long double, luaL_checknumber, lua_pushnumber>
{};

template<>
struct type_utility<const char*>
{
    using target_type = const char*;

    static target_type check(lua_State* L, int i)
    {
        return luaL_checkstring(L, i);
    }

    static void push(lua_State* L, target_type val)
    {
        lua_pushstring(L, val);
    }
};

//! As for function types, no read function is realized.
//! To pass them to C functions, use `arg_wrapper` or `apply_stack`.
template<class R, class... Args>
struct type_utility<R(*)(Args...)> {
    using target_type = R(*)(Args...);

    template<class U = R>
    static void push(lua_State* L, target_type fun)
    {
        lua_pushlightuserdata(L, static_cast<void*>(&fun));
        lua_pushcclosure(
            L,
            +[](lua_State* L) -> int {
                auto fun = static_cast<target_type*>(
                    lua_touserdata(L, lua_upvalueindex(1)));

                return apply_stack(L, *fun);;
            },
            1);
    }
};

/*!
 * Data type carrying arguments passed from Lua environment.
 * \tparam T The target type.
 * \tparam D The parameter to decorate the class. This is mainly used for argments of function types.
 */
template<class T, class D = void>
class arg_wrapper
{
public:
    using target_type = T;

    arg_wrapper(lua_State* L, int i)
        : m_arg(type_utility<target_type>::check(L, i))
    {}

    ~arg_wrapper() = default;

    target_type get() { return m_arg; }

private:
    target_type m_arg;
};

template<class D, class R, class... Args>
class arg_wrapper<R(*)(Args...), D>
{
public:
    using target_type = R(*)(Args...);

    arg_wrapper(lua_State* L, int i)
        : is_responsible(!m_Lstate)
    {
        m_Lstate = L;

        luaL_checktype(m_Lstate, i, LUA_TFUNCTION);

        lua_pushvalue(m_Lstate, i);
        m_reg_idx = luaL_ref(m_Lstate, LUA_REGISTRYINDEX);
    }

    arg_wrapper(const arg_wrapper&) = delete;

    arg_wrapper(arg_wrapper&& src)
        : is_responsible(src.is_responsible)
    {
        src.is_responsible = false;
    }

    ~arg_wrapper()
    {
        if(is_responsible) {
            luaL_unref(m_Lstate, LUA_REGISTRYINDEX, m_reg_idx);
            m_Lstate = nullptr;
        }
    }

    arg_wrapper& operator=(const arg_wrapper&) = delete;

    arg_wrapper& operator=(arg_wrapper&& src)
    {
        if(!is_responsible)
            is_responsible = src.is_responsible;

        src.is_responsible = false;
    }

    template<class U=R>
    auto get()
        -> typename std::enable_if<
            !std::is_same<U,void>::value, target_type>::type
    {
        return +[](Args... args) -> R {
            int oldtop = lua_gettop(m_Lstate);

            // ensure there is enough room in the Lua stack.
            lua_checkstack(m_Lstate, sizeof...(Args)+1);

            lua_rawgeti(m_Lstate, LUA_REGISTRYINDEX, m_reg_idx);

            auto dummy = {
                (type_utility<Args>::push(m_Lstate, std::forward<Args>(args)),0)
                ...
            };
            static_cast<volatile void>(dummy);

            if (lua_pcall(m_Lstate, sizeof...(Args), LUA_MULTRET, 0)) {
                //! \todo Do not want to use std::cerr to handle error messages.
                std::cerr << lua_tostring(m_Lstate, -1) << std::endl;
                lua_pop(m_Lstate, 1);
                return R();
            }

            auto ret = type_utility<R>::check(m_Lstate, oldtop+1);
            lua_pop(m_Lstate, lua_gettop(m_Lstate)-oldtop);
            return ret;
        };
    }

    template<class U=R>
    auto get()
        -> typename std::enable_if<
            std::is_same<U,void>::value, target_type>::type
    {
        return +[](Args... args) -> R {
            int oldtop = lua_gettop(m_Lstate);
            lua_rawgeti(m_Lstate, LUA_REGISTRYINDEX, m_reg_idx);
            auto dummy = {
                (type_utility<Args>::push(m_Lstate, std::forward<Args>(args)),0)
                ... };
            static_cast<void>(dummy);

            if (lua_pcall(m_Lstate, sizeof...(Args), LUA_MULTRET, 0)) {
                std::cerr << lua_tostring(m_Lstate, -1) << std::endl;
                lua_pop(m_Lstate, 1);
                return;
            }

            lua_pop(m_Lstate, lua_gettop(m_Lstate)-oldtop);
        };
    }

private:
    bool is_responsible;

    static lua_State* m_Lstate;
    static int m_reg_idx;
};

template<class D, class R, class... Args>
lua_State* arg_wrapper<R(*)(Args...), D>::m_Lstate = nullptr;

template<class D, class R, class... Args>
int arg_wrapper<R(*)(Args...), D>::m_reg_idx;

//! The implementation of the function apply_stack.
template<class D, class F, size_t... idx>
auto apply_stack_impl(
    lua_State* L, F f, std::integer_sequence<size_t, idx...>&&)
    -> typename std::enable_if<
        !std::is_same<void,typename utils::function_signature<F>::result_type>::value,
        int>::type
{
    using arg_tuple_t = typename utils::function_signature<F>::arg_tuple_t;
    auto arg_tup = std::make_tuple(
        arg_wrapper<
            std::tuple_element_t<idx,arg_tuple_t>,
            utils::with_index<idx, D> >(L, idx+1)
        ...);
    // In case the argmentlist is empty, arg_tup may not be used.
    static_cast<void>(arg_tup);

    auto ret = f(std::get<idx>(arg_tup).get()...);
    type_utility<std::remove_cv_t<decltype(ret)> >::push(L, ret);
    return 1;
}

template<class D, class F, size_t... idx>
auto apply_stack_impl(
    lua_State* L, F f, std::integer_sequence<size_t, idx...>&&)
    -> typename std::enable_if<
        std::is_same<void,typename utils::function_signature<F>::result_type>::value,
        int>::type
{
    using arg_tuple_t = typename utils::function_signature<F>::arg_tuple_t;
    auto arg_tup = std::make_tuple(
        arg_wrapper<
            std::tuple_element_t<idx,arg_tuple_t>,
            utils::with_index<idx, D> >(L, idx+1)
        ...);
    // In case the argmentlist is empty, arg_tup may not be used.
    static_cast<void>(arg_tup);

    f(std::get<idx>(arg_tup).get()...);

    return 0;
}

template<class F, class D>
int apply_stack(lua_State* L, F f)
{
    return apply_stack_impl<D>(
        L, std::forward<F>(f),
        std::make_index_sequence<utils::function_signature<F>::arity>{});
}

/*!
 * Static wrapper of C functions to create functions exposed in Lua.
 * \tparam F the type signature of the wrapped function.
 * \tparam f the wrapped function.
 */
template<class F, std::decay_t<F> f>
int fun_wrapper(lua_State* L)
{
    return apply_stack<std::decay_t<F>,utils::singleton_type<F,f> >(L, f);
}

/************************************
 *** Tools to exhibit enum to Lua ***
 ************************************/
/*!
 * The data structure to represent an entry of an enum type
 */
template<class E,
        class = std::enable_if_t<std::is_enum<E>::value> >
struct enum_entry
{
    using type = E;
    const char* name;
    const E value;

    constexpr enum_entry(const char* n, E val)
      : name(n), value(val)
    {}
};

/*!
 * The implementaion of `push_enum_table` function.
 */
inline int push_enum_table_impl(lua_State*)
{
    return 1;
}

template<class D, class... Ds>
inline int push_enum_table_impl(lua_State* L, D&& head, Ds&&... rem)
{
    lua_pushstring(L, head.name);
    tsukimade::type_utility<typename std::remove_reference_t<D>::type>::push(L, head.value);
    lua_settable(L, -3);

    return push_enum_table_impl(L, rem...);
}

/*!
 * The function pushing a table containing key-value pairs of an enum type.
 */
template<class... Ds>
int push_enum_table(lua_State* L, Ds&&... entries)
{
    lua_createtable(L, 0, sizeof...(Ds));

    return push_enum_table_impl(L, entries...);
}

//! Syntactic sugar
#define MK_ENUMENTRY(v) enum_entry<std::remove_reference_t<decltype(v)> >(#v, v)

} // namespace tsukimade

#endif
