#include "random.hpp"

#include <gtest/gtest.h>
#include <string>
TEST(RandomTest, StringGenerator)
{
    std::string str = random_string(10);
    ASSERT_EQ(str.size(), 10);
}
TEST(RandomTest, StringGeneratorLen0)
{
    std::string str = random_string(0);
    ASSERT_EQ(str.size(), 0);
}
