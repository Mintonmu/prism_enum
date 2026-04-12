#ifndef __PRISM_ENUM_H__
#define __PRISM_ENUM_H__

#include <cstddef>
#include <string>
#include <string_view>

namespace prism_enum {

namespace detail {
constexpr auto npos = std::string_view::npos;

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

constexpr auto string2Enum() { return; }
} // namespace prism_enum
#endif // __PRISM_ENUM_H__