#include "singleton.hpp"
#include <future>
#include <gtest/gtest.h>
#include <string>
#include <thread>

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
TEST(Singleton, GetInstance)
{
    auto future_t1 = std::async(ThreadBarWithMutex);
    auto future_t2 = std::async(ThreadFoo);
    ASSERT_EQ(future_t1.get(), future_t2.get());
}
