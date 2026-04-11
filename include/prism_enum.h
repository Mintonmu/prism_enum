#ifndef __PRISM_ENUM_H__
#define __PRISM_ENUM_H__

#include <string>

namespace prism_enum {
template <typename E> constexpr auto enum2String(E enum_value) { return; }
constexpr auto string2Enum(std::string &str) { return; }
} // namespace prism_enum
#endif // __PRISM_ENUM_H__