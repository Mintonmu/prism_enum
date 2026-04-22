#include <catch2/catch_test_macros.hpp>
#include "prism_enum.h"

// Scoped Enum with custom underlying type
enum class CustomCounter : short { Zero = 0, One = 1, Two = 2, Negative = -1 };

// Empty Scoped Enum
enum class EmptyScoped {};

// Empty Unscoped Enum
enum EmptyUnscoped {};

namespace prism_enum {
template <> struct enum_traits<CustomCounter> { 
    static constexpr int min = -2; 
    static constexpr int max = 3; 
};
template <> struct enum_traits<EmptyScoped> { 
    static constexpr int min = 0; 
    static constexpr int max = 0; 
};
template <> struct enum_traits<EmptyUnscoped> { 
    static constexpr int min = 0; 
    static constexpr int max = 0; 
};
}

TEST_CASE("edge cases scoped vs unscoped", "[edge_cases]") {
    SECTION("is_scoped_enum differentiates correct empty ones") {
        REQUIRE(prism_enum::detail::is_scoped_enum_v<CustomCounter> == true);
        REQUIRE(prism_enum::detail::is_scoped_enum_v<EmptyScoped> == true);
        REQUIRE(prism_enum::detail::is_scoped_enum_v<EmptyUnscoped> == false);
    }
    SECTION("negative values in bounded enum_traits") {
        constexpr auto nameNegative = prism_enum::enum2String<CustomCounter::Negative>();
        REQUIRE((nameNegative == "CustomCounter::Negative" || nameNegative == "Negative"));
        
        auto opt = prism_enum::tryString2Enum<CustomCounter>(nameNegative);
        REQUIRE(opt.has_value());
        REQUIRE(opt.value() == CustomCounter::Negative);
    }
    SECTION("empty enums") {
        auto opt1 = prism_enum::tryString2Enum<EmptyScoped>("AnyString");
        REQUIRE(opt1.has_value() == false);

        auto opt2 = prism_enum::tryString2Enum<EmptyUnscoped>("AnyString");
        REQUIRE(opt2.has_value() == false);
    }
    SECTION("string2Enum fallback value") {
        REQUIRE(prism_enum::string2Enum<CustomCounter>("NonExistent", CustomCounter::Zero) == CustomCounter::Zero);
        // By default string2Enum uses 0 as fallback, which is Zero here
        REQUIRE(prism_enum::string2Enum<CustomCounter>("NonExistent2") == CustomCounter::Zero);
    }
}
