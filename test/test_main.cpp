#include <catch2/catch_test_macros.hpp>
#include "prism_enum.h"

// 1. Scoped Enum
enum class Color { Red, Green, Blue };

// 2. Unscoped Enum with underlying type
enum Status : int { Ok = 200, NotFound = 404 };

// 3. Plain Unscoped Enum
enum PlainUnscoped { P1, P2, P3 };

// 4. Nested Enum in Struct
struct Wrapper { enum class Nested { Item1, Item2 }; };

// 5. Enum in Union
union UnionWrapper {
    int x;
    enum InsideUnion { U_A, U_B };
};

namespace prism_enum {
template <> struct enum_traits<Status> { static constexpr int min = 200; static constexpr int max = 405; };
template <> struct enum_traits<PlainUnscoped> { static constexpr int min = 0; static constexpr int max = 2; };
template <> struct enum_traits<Wrapper::Nested> { static constexpr int min = 0; static constexpr int max = 1; };
template <> struct enum_traits<UnionWrapper::InsideUnion> { static constexpr int min = 0; static constexpr int max = 1; };
}

TEST_CASE("prism_enum traits and type properties", "[traits]") {
    SECTION("is_scoped_enum_v identifies scoped enums correctly") {
        REQUIRE(prism_enum::detail::is_scoped_enum_v<Color> == true);
        REQUIRE(prism_enum::detail::is_scoped_enum_v<Status> == false);
        REQUIRE(prism_enum::detail::is_scoped_enum_v<PlainUnscoped> == false);
        REQUIRE(prism_enum::detail::is_scoped_enum_v<Wrapper::Nested> == true);
    }
    SECTION("is_unscoped_enum_v identifies unscoped enums correctly") {
        REQUIRE(prism_enum::detail::is_unscoped_enum_v<Color> == false);
        REQUIRE(prism_enum::detail::is_unscoped_enum_v<Status> == true);
        REQUIRE(prism_enum::detail::is_unscoped_enum_v<PlainUnscoped> == true);
        REQUIRE(prism_enum::detail::is_unscoped_enum_v<Wrapper::Nested> == false);
        REQUIRE(prism_enum::detail::is_unscoped_enum_v<UnionWrapper::InsideUnion> == true);
    }
}

TEST_CASE("prism_enum string conversion for diverse types", "[prism]") {
    SECTION("Scoped enum (enum class Color)") {
        constexpr auto name = prism_enum::enum2String<Color::Green>();
        REQUIRE((name == "Color::Green" || name == "Green"));
        
        // Use exact compiler string extraction representation for robust tests
        REQUIRE(prism_enum::tryString2Enum<Color>(name).value() == Color::Green);
    }

    SECTION("Unscoped enum with underlying type (Status : int)") {
        constexpr auto name = prism_enum::enum2String<Status::Ok>();
        REQUIRE((name == "Status::Ok" || name == "Ok"));
        
        REQUIRE(prism_enum::tryString2Enum<Status>(name).value() == Status::Ok);
    }

    SECTION("Plain unscoped enum (PlainUnscoped)") {
        constexpr auto name = prism_enum::enum2String<P2>();
        REQUIRE((name == "PlainUnscoped::P2" || name == "P2"));
        auto opt = prism_enum::tryString2Enum<PlainUnscoped>(name);
        REQUIRE(opt.has_value());
        REQUIRE(opt.value() == P2);
    }

    SECTION("Nested enum in Struct (Wrapper::Nested)") {
        constexpr auto name = prism_enum::enum2String<Wrapper::Nested::Item2>();
        REQUIRE_FALSE(name.empty());
        auto opt = prism_enum::tryString2Enum<Wrapper::Nested>(name);
        REQUIRE(opt.has_value());
        REQUIRE(opt.value() == Wrapper::Nested::Item2);
    }

    SECTION("Enum inside Union (UnionWrapper::InsideUnion)") {
        constexpr auto name = prism_enum::enum2String<UnionWrapper::U_A>();
        REQUIRE_FALSE(name.empty());
        auto opt = prism_enum::tryString2Enum<UnionWrapper::InsideUnion>(name);
        REQUIRE(opt.has_value());
        REQUIRE(opt.value() == UnionWrapper::U_A);
    }
}
