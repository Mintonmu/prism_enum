#ifndef __PRISM_ENUM_H__
#define __PRISM_ENUM_H__

#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>

namespace prism_enum {

namespace detail {
template <typename T, bool = std::is_enum_v<T>>
struct is_scoped_enum : std::false_type {};

template <typename T>
struct is_scoped_enum<T, true>
    : std::bool_constant<!std::is_convertible_v<T, std::underlying_type_t<T>>> {
};

template <typename T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

template <typename T, bool = std::is_enum_v<T>>
struct is_unscoped_enum : std::false_type {};

template <typename T>
struct is_unscoped_enum<T, true>
    : std::bool_constant<std::is_convertible_v<T, std::underlying_type_t<T>>> {
};

template <typename T>
inline constexpr bool is_unscoped_enum_v = is_unscoped_enum<T>::value;
} // namespace detail

template <typename E>
struct enum_traits;

namespace detail {
constexpr auto npos = std::string_view::npos;

template <typename E, typename = void>
struct enum_traits_bounds {
  static constexpr int min = -128;
  static constexpr int max = 128;
};

template <typename E>
struct enum_traits_bounds<E, std::void_t<decltype(enum_traits<E>::min),
                                         decltype(enum_traits<E>::max)>> {
  static constexpr int min = static_cast<int>(enum_traits<E>::min);
  static constexpr int max = static_cast<int>(enum_traits<E>::max);
};

constexpr std::string_view extract_enum_name(std::string_view signature,
                                             std::string_view key) {
  const auto begin = signature.find(key);
  if (begin == npos) {
    return {};
  }

  const auto value_begin = begin + key.size();
  const auto value_end = signature.find_first_of(";]>", value_begin);
  if (value_end == npos || value_end <= value_begin) {
    return {};
  }

  auto value = signature.substr(value_begin, value_end - value_begin);
  while (!value.empty() && value.front() == ' ') {
    value.remove_prefix(1);
  }

  if (value.find('(') != npos) {
    return {};
  }

  const auto scope = value.rfind("::");
  if (scope != npos) {
    value.remove_prefix(scope + 2);
  }

  return value;
}
} // namespace detail

template <auto V> constexpr std::string_view get_raw_name() {
  std::string_view name;
#if defined(__clang__) || defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  return detail::extract_enum_name(name, "V = ");
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  return detail::extract_enum_name(name, "<");
#else
  (void)name;
  return {};
#endif
}

template <auto V> constexpr std::string_view enum2String() {
  return get_raw_name<V>();
}

namespace detail {
template <typename E, int Min, std::size_t... I>
constexpr std::optional<E>
try_string2enum_impl(std::string_view input, std::index_sequence<I...>) {
  std::optional<E> result;

  (([&] {
     constexpr int candidate_value = Min + static_cast<int>(I);
     constexpr E candidate = static_cast<E>(candidate_value);
     constexpr auto name = enum2String<candidate>();

     if (!name.empty() && name == input) {
       result = candidate;
     }
   }()),
   ...);

  return result;
}
} // namespace detail

template <typename E>
constexpr std::optional<E> tryString2Enum(std::string_view str_view) {
  static_assert(std::is_enum_v<E>, "tryString2Enum requires an enum type.");
  constexpr int min = detail::enum_traits_bounds<E>::min;
  constexpr int max = detail::enum_traits_bounds<E>::max;
  static_assert(max >= min, "Invalid enum scan range.");

  return detail::try_string2enum_impl<E, min>(
      str_view, std::make_index_sequence<static_cast<std::size_t>(max - min + 1)>{});
}

template <typename E>
constexpr E string2Enum(std::string_view str_view,
                        E fallback = static_cast<E>(0)) {
  if (const auto value = tryString2Enum<E>(str_view)) {
    return *value;
  }
  return fallback;
}
} // namespace prism_enum
#endif // __PRISM_ENUM_H__