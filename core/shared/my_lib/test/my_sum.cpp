#include "mylib.h"

#include <catch2/catch_all.hpp>
TEST_CASE("sum_test", "[sum_fun]") { REQUIRE(Sum(2, 2) == 4); }
