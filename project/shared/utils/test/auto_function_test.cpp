#include "auto_function.hpp"
#include <gtest/gtest.h>

int add(int a, int b) {
  return a + b;
}

float multiple(float a, float b, float c) {
  return a * b * c;
}

TEST(AutoFunction, AutoClass) {
  ASSERT_EQ(Call(add, 1, 2), 3);
  ASSERT_EQ(Call(multiple, 1, 2, 3), 6.0);
}

class TestClass {
 public:
  int add(int a, int b) { return a + b; }

  float multiple(float a, float b, float c) { return a * b * c; }
};

TEST(AutoFunction, AutoClassWithMemberFunc) {
  TestClass* t;
  auto val = Call(&TestClass::add, t, 2, 1);
  ASSERT_EQ(val, 3);
}
