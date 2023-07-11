#include <gtest/gtest.h>

#include <string>

#include "random.hpp"

TEST(Random, StringGenerator) {
  std::string str = random_string(10);
  ASSERT_EQ(str.size(), 10);
}

TEST(Random, StringGeneratorLen0) {
  std::string str = random_string(0);
  ASSERT_EQ(str.size(), 0);
}
