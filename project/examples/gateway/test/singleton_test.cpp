#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "singleton.hpp"
#include <doctest/doctest.h>
#include <future>
#include <string>

class SingletonDemo : public Singleton<SingletonDemo>
{
};
std::string ThreadFoo()
{
  // Following code emulates slow initialization.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto singleton = SingletonDemo::GetInstance("foo");
  return singleton->value();
}
std::string ThreadBarWithMutex()
{
  // Following code emulates slow initialization.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  auto singleton = SingletonDemo::GetInstance("Bar");

  return singleton->value();
}
TEST_CASE("thread safe singleton")
{
  auto future_t1 = std::async(ThreadBarWithMutex);
  auto future_t2 = std::async(ThreadFoo);
  CHECK_EQ(future_t1.get(), future_t2.get());
}
