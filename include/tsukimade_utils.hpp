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

    T& operator T() { return raw_; }

    const T& operator T() const { return raw_; }

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

} // namespace utils
} // namespace tsukimade

#endif
