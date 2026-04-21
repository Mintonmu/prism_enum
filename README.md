# prism_enum

A lightweight, header-only C++17 library for compile-time and runtime enum reflection. It allows you to convert enum values to strings and parse strings back into enum values without the need for macros, boilerplate code, or external code generation tools.

## Features

*   **Header-Only**: Just include `prism_enum.h` and you are ready to go.
*   **No Macros Needed**: Works with standard, unmodified C++ enums (`enum` and `enum class`).
*   **Compile-Time & Runtime**: Leverages compiler intrinisics (`__PRETTY_FUNCTION__`, `__FUNCSIG__`) and `constexpr` evaluation.
*   **Build System Agnostic**: Easily integrates via Meson, Zig, CMake, or by simply dropping the headers into your project.

## Usage

```cpp
#include <iostream>
#include "prism_enum.h"

// Define a normal enum
enum class Color {
    Red,
    Green = 10,
    Blue
};

int main() {
    // 1. Enum to String 
    // Evaluates at compile-time and returns a std::string_view
    constexpr auto red_str = prism_enum::enum2String<Color::Red>();
    std::cout << "Color::Red -> " << red_str << "\n"; // Output: Red

    // 2. String to Enum (Exact match)
    // Tries to map a runtime string back to an enum value (defaults to casting 0 if not found)
    auto blue_enum = prism_enum::string2Enum<Color>("Blue");
    
    // 3. String to Enum (Safe Optional)
    // Returns std::optional<Color> for safe parsing
    if (auto green_opt = prism_enum::tryString2Enum<Color>("Green")) {
        std::cout << "Successfully parsed Green!\n";
    }

    return 0;
}
```

### Extending the Search Range

By default, `prism_enum` scans integer values from `-128` to `128` (compile-time generation loop) to build its lookup cache and find matchable strings. If your enum has values outside this range, you can specialize `prism_enum::enum_traits` to specify custom bounds for that specific enum:

```cpp
enum class Status {
    Ok = 200,
    NotFound = 404,
    InternalError = 500
};

namespace prism_enum {
template <>
struct enum_traits<Status> {
    static constexpr int min = 200;
    static constexpr int max = 505;
};
}
```

## Integration

### Meson

Add `prism_enum` as a Meson subproject (e.g., in `subprojects/prism_enum`) and link the dependency:

```meson
prism_enum_proj = subproject('prism_enum')
prism_enum_dep = prism_enum_proj.get_variable('prism_enum_dep')

executable('my_app', 
  'main.cpp', 
  dependencies: prism_enum_dep
)
```

### Zig

Since it provides a Zig module out of the box, add it to your `build.zig.zon` and use it in `build.zig`:

```zig
const prism_enum_dep = b.dependency("prism_enum", .{});
const prism_enum_mod = prism_enum_dep.module("prism_enum");

exe.root_module.addImport("prism_enum", prism_enum_mod);
```

### CMake / Direct Integration

Because it's a pure header-only library, you don't even need a build system script. Just copy the `include/` directory to your project and add it to your compiler's include path (`-I include/`).
