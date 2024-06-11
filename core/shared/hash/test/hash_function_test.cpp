#include "hash_function.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define SHA_256_HELLO_HEX                                                      \
    "8F4EC1811C6C4261C97A7423B3A56D69F0F160074F39745AF20BB5FCF65CCF78"
#define sha1_HELLO_HEX "16AD856B462E68F965F6E93F66282A7AE891FDBC"
#define MD5_HELLO_HEX "98F97A791EF1457579A5B7E88A495063"
#define HELLO_WORLD "Hello,World!"
TEST(HashMethodTest, Sha256Cstr)
{
    hash_function func;
    auto result = func.sha_256_hash(HELLO_WORLD);
    ASSERT_STREQ(result.c_str(), SHA_256_HELLO_HEX);
}
TEST(HashMethodTest, Sha256Str)
{
    hash_function func;
    std::string demo = std::string(HELLO_WORLD);
    auto result = func.sha_256_hash(demo);
    ASSERT_STREQ(result.c_str(), SHA_256_HELLO_HEX);
    ASSERT_EQ(64, result.length());
}
TEST(HashMethodTest, Sha1Str)
{
    hash_function func;
    std::string demo = std::string(HELLO_WORLD);
    auto result = func.sha1_hash(demo);
    ASSERT_STREQ(result.c_str(), sha1_HELLO_HEX);
    ASSERT_EQ(40, result.length());
}
TEST(HashMethodTest, Sha1Cstr)
{
    hash_function func;
    auto result = func.sha1_hash(HELLO_WORLD);
    ASSERT_STREQ(result.c_str(), sha1_HELLO_HEX);
}

TEST(HashMethodTest, md5Str)
{
    hash_function func;
    std::string demo = std::string(HELLO_WORLD);
    auto result = func.md5_hash(demo);
    ASSERT_STREQ(result.c_str(), MD5_HELLO_HEX);
    ASSERT_EQ(32, result.length());
}
TEST(HashMethodTest, md5Cstr)
{
    hash_function func;
    auto result = func.md5_hash(HELLO_WORLD);
    ASSERT_STREQ(result.c_str(), MD5_HELLO_HEX);
}