#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <my_lib/lib.hpp>

TEST_CASE("some_fun") { REQUIRE(some_fun() == 0); }

TEST_CASE("nJson request test") { REQUIRE(http_get_demo() == 200); }
TEST_CASE("https nJson support test")
{
    REQUIRE(http_get_demo() == 200);
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    fmt::print("openssl enabled");
#else
    fmt::print("openssl disabled");
#endif
}