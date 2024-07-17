#include "../auto_function.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

int add(int a, int b) { return a + b; }
float multiple(float a, float b, float c) { return a * b * c; }

class TestClass
{
public:
    int add(int a, int b) { return a + b; }
    float multiple(float a, float b, float c) { return a * b * c; }
};

TEST_CASE("AutoFunction AutoClass", "[AutoFunction]")
{
    REQUIRE(Call(add, 1, 2) == 3);
    REQUIRE(Call(multiple, 1, 2, 3) == 6.0f);
}

TEST_CASE("AutoFunction AutoClassWithMemberFunc", "[AutoFunction]")
{
    TestClass t;
    auto val = Call(&TestClass::add, &t, 2, 1);
    REQUIRE(val == 3);
}
