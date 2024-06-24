#include "../../singleton.hpp"
#include "singleton.hpp"
#include <gtest/gtest.h>
#include <nanobench.h>
class Foo : public SingletonDclp<Foo>
{
public:
    explicit Foo(int n) : n_{n} {}
    void Bar() {}

private:
    int n_;
};

TEST(Benchmarks, singleton)
{

    ankerl::nanobench::Bench().run("get bar",
                                   [&]()
                                   {
                                       Foo::Construct(17);
                                       Foo::GetInstance()->Bar();
                                       Foo::Destruct();
                                   });
}
static std::atomic_uint32_t init{0};
class Counter : public Singleton<Counter>
{
public:
    Counter() { ++init; }

    void Add()
    {
        ++count_;
        std::cout << "count: " << count_ << std::endl;
    }
    std::uint32_t GetCount() const
    {
        std::cout << "count: " << count_ << std::endl;
        return count_;
    }

private:
    std::atomic_uint32_t count_{0};
};
TEST(Benchmarks, singletonAtomic)
{

    ankerl::nanobench::Bench().run("get bar",
                                   [&]()
                                   {
                                       Foo::Construct(17);
                                       Foo::GetInstance()->Bar();
                                       Foo::Destruct();
                                   });
}