#include "human_readable.hpp"
#include <catch2/catch_test_macros.hpp>
constexpr size_t IntegerRepresentableBoundary()
{
    static_assert(std::numeric_limits<double>::radix, 2,
                  "Method only valid for binary floating point format.");
    return size_t{2} << (std::numeric_limits<double>::digits - 1);
}
TEST_CASE("HumanReadable output stream operator test")
{
    HumanReadable hr;
    hr.size = 1024;

    std::ostringstream oss;
    oss << hr;

    REQUIRE(oss.str() == "1KB (1024)");
}
TEST_CASE("Human Readable cast tests")
{
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() - 2})) ==
            size_t{IntegerRepresentableBoundary() - 2});
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() - 1})) ==
            size_t{IntegerRepresentableBoundary() - 1});
    REQUIRE(static_cast<size_t>(
                NarrowCast<double>(size_t{IntegerRepresentableBoundary()})) ==
            size_t{IntegerRepresentableBoundary()});
    REQUIRE_THROWS_AS(
        NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 1}),
        std::exception);
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() + 2})) ==
            size_t{IntegerRepresentableBoundary() + 2});
    REQUIRE_THROWS_AS(
        NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 3}),
        std::exception);
    REQUIRE(static_cast<size_t>(NarrowCast<double>(
                size_t{IntegerRepresentableBoundary() + 4})) ==
            size_t{IntegerRepresentableBoundary() + 4});
    REQUIRE_THROWS_AS(
        NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 5}),
        std::exception);
}
