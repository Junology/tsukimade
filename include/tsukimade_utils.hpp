/*!
 * \file tsukimade_utils.hpp
 * \author Jun Yoshida
 * \brief
 *     This header contains several utility tools for types.
 *
 * \copyright (c) 2019 Jun Yoshida.
 * The project is released under MIT License.
 * \date April 27, 2019: created
 */

#ifndef TSUKIMADE_UTILITY_HEADER
#define TSUKIMADE_UTILITY_HEADER

#include <tuple>
#include <type_traits>

namespace tsukimade {
namespace utils {


/*!
 * Recognizing the type signature of a function type.
 * It would be better in C++17 or later to use `std::invoke_result`.
 * \tparam F function type; same as in std::function<>;
 */
template<class F>
struct function_signature;

template<class R, class... Args>
struct function_signature<R(Args...)> {
    using result_type = R;
    using arg_tuple_t = std::tuple<Args...>;
    static constexpr const size_t arity = sizeof...(Args);
};

template<class R, class... Args>
struct function_signature<R(*)(Args...)>
    : function_signature<R(Args...)>
{};


/*!
 * Remove all const/volatile qualifiers appearing in a type.
 * In other words, this applies std::remove_cv recursively.
 */
template<class T>
struct annihilate_cv
{
    using type = T;
};

template<class T>
struct annihilate_cv<T const>
{
    using type = typename annihilate_cv<T>::type;
};

template<class T>
struct annihilate_cv<T volatile>
{
    using type = typename annihilate_cv<T>::type;
};

template<class T>
struct annihilate_cv<T*>
{
    using type = typename annihilate_cv<T>::type;
};

template<class T>
struct annihilate_cv<T&>
{
    using type = typename annihilate_cv<T>::type;
};

template<class T>
using annihilate_cv_t = typename annihilate_cv<T>::type;


/*!
 * A class to represent a class with decoration.
 * It can be casted to the original type.
 * \tparam D A class decorating the original type.
 * \tparam T An original type.
 */
template<class D, class T>
class with_type {
public:
    using raw_type = T;

    with_type(const T& init)
        : raw_(init) {}

    with_type(T&& init)
        : raw_(std::move<T>(init)) {}

    template<class E>
    with_type(const with_type<E,T>& src)
        : raw_(src.raw_) {}

    template<class E>
    with_type(with_type<E,T>&& src)
        : raw_(std::move<T>(src.raw_)) {}

    ~with_type() = default;

    operator T() const { return raw_; }

    T& operator=(const T& val) { raw_ = val; return raw_; }

    T& operator=(T&& val) { raw_ = std::move<T>(val); return raw_; }

private:
    T raw_;
};

/*!
 * A class to represent a class decorated with an index.
 * \tparam n An index to decorating the original type.
 * \tparam T An original type.
 */
template<size_t n, class T>
using with_index = with_type<std::integral_constant<size_t, n>, T>;


/*!
 * A class with a statically constant value.
 * It can be seen as a "singleton consisting of the given value."
 * So, in spite of its name, it is irrelevant to Singleton Pattern explained in GoF book.
 */
// Want to write `template<auto x>`, which is available for C++17 or later.
template<class T, std::decay_t<T> x>
struct singleton_type
{
    using value_type = std::decay_t<T>;

    static const constexpr value_type value = x;
};

} // namespace utils
} // namespace tsukimade

#endif
