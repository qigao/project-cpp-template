#include <atomic>
#include <latch>
#include <thread>
#include <vector>

#include "../singleton.hpp"

#include <gtest/gtest.h>

static std::atomic_uint32_t init{0};

class Counter : public Singleton<Counter>
{
public:
    Counter() { ++init; }

    void Add() { ++count_; }
    std::uint32_t GetCount() const { return count_; }

private:
    std::atomic_uint32_t count_{0};
};

TEST(SingletonTest, multiThread)
{
    auto const count = std::thread::hardware_concurrency();
    std::cout << "hardware: " << count << std::endl;
    std::latch block{static_cast<std::ptrdiff_t>(count)};
    std::vector<std::thread> threads;

    threads.reserve(count);
    for (auto i = 0u; i < count; ++i)
    {
        threads.emplace_back(
            [&block]
            {
                block.arrive_and_wait();
                Counter::Construct();
                Counter::GetInstance()->Add();
            });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    EXPECT_EQ(init, 1);
    EXPECT_EQ(Counter::GetInstance()->GetCount(), count);
}
