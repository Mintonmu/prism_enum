#ifndef __PRISM_IMPL_H__
#define __PRISM_IMPL_H__

#include <type_traits>
namespace prism_enum {
namespace detail {
template <typename T, bool = std::is_enum_v<T>>
struct is_scoped_enum : std::false_type {};

template <typename T>
struct is_scoped_enum<T, true>
    : std::bool_constant<!std::is_convertible_v<T, std::underlying_type_t<T>>> {
};

template <typename T>
using is_scoped_enum_t = typename is_scoped_enum<T>::value;

template <typename T, bool = std::is_enum_v<T>>
struct is_unscoped_enum : std::false_type {};

template <typename T>
struct is_unscoped_enum<T, true>
    : std::bool_constant<std::is_convertible_v<T, std::underlying_type_t<T>>> {
};

template <typename T>
using is_unscoped_enum_t = typename is_unscoped_enum<T>::value;
} // namespace detail
} // namespace prism_enum
#endif // __PRISM_IMPL_H__