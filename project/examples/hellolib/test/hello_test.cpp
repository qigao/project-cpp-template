#include "hellolib.h"

#include <gtest/gtest.h>
using namespace hello;

TEST(HelloTest, SaySomething)
{
  hellolib lib;
  ASSERT_EQ(lib.saySomething("Hello World!"), 0);
}
TEST(HelloTest, SaySomethingEmpty)
{
  hellolib lib;
  ASSERT_EQ(lib.saySomething(""), 1);
}
TEST(HelloTest, SaySomethingHashed)
{
#ifdef WITH_OPENSSL
  hellolib lib;
  ASSERT_EQ(lib.saySomethingHashed("Hello World!"), 0);
#endif
}
TEST(HelloTest, SaySomethingHashedEmpty)
{
#ifdef WITH_OPENSSL
  hellolib lib;
  ASSERT_GT(lib.saySomethingHashed(""), 1);
#endif
}
