#include "hash_function.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define SHA_256_HELLO_HEX                                                      \
    "8f4ec1811c6c4261c97a7423b3a56d69f0f160074f39745af20bb5fcf65ccf78"
TEST(HashMethodTest, sodiumSha256Cstr)
{
    hash_function func;
    auto result = func.sha_256_hash("Hello,World!");
    ASSERT_STREQ(result.c_str(), SHA_256_HELLO_HEX);
}
TEST(HashMethodTest, sodiumSha256Str)
{
    hash_function func;
    std::string demo = std::string("Hello,World!");
    auto result = func.sha_256_hash(demo);
    ASSERT_STREQ(result.c_str(), SHA_256_HELLO_HEX);
    ASSERT_EQ(64, result.length());
}