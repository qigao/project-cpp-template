#include "hash_function.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#define SHA_256_HELLO_HEX                                                      \
    "8F4EC1811C6C4261C97A7423B3A56D69F0F160074F39745AF20BB5FCF65CCF78"
#define SHA1_HELLO_HEX "16AD856B462E68F965F6E93F66282A7AE891FDBC"
#define MD5_HELLO_HEX "98F97A791EF1457579A5B7E88A495063"
#define HELLO_WORLD "Hello,World!"

TEST_CASE("HashMethodTest Sha256Cstr", "[hash_func]")
{
    hash_function func;
    auto result = func.sha_256_hash(HELLO_WORLD);
    REQUIRE_THAT(result, Catch::Matchers::Equals(SHA_256_HELLO_HEX));
}
TEST_CASE("HashMethodTest Sha256Str", "[hash_func]")
{
    hash_function func;
    std::string demo = std::string(HELLO_WORLD);
    auto result = func.sha_256_hash(demo);
    REQUIRE_THAT(result, Catch::Matchers::Equals(SHA_256_HELLO_HEX));
    REQUIRE(result.length() == 64);
}
TEST_CASE("HashMethodTest Sha1Str", "[hash_func]")
{
    hash_function func;
    std::string demo = std::string(HELLO_WORLD);
    auto result = func.sha1_hash(demo);
    REQUIRE_THAT(result, Catch::Matchers::Equals(SHA1_HELLO_HEX));
    REQUIRE(result.length() == 40);
}
TEST_CASE("HashMethodTest Sha1Cstr", "[hash_func]")
{
    hash_function func;
    auto result = func.sha1_hash(HELLO_WORLD);
    REQUIRE_THAT(result, Catch::Matchers::Equals(SHA1_HELLO_HEX));
}

TEST_CASE("HashMethodTest md5Str", "[hash_func]")
{
    hash_function func;
    std::string demo = std::string(HELLO_WORLD);
    auto result = func.md5_hash(demo);
    REQUIRE_THAT(result, Catch::Matchers::Equals(MD5_HELLO_HEX));
    REQUIRE(result.length() == 32);
}
TEST_CASE("HashMethodTest md5Cstr", "[hash_func]")
{
    hash_function func;
    auto result = func.md5_hash(HELLO_WORLD);
    REQUIRE_THAT(result, Catch::Matchers::Equals(MD5_HELLO_HEX));
}
