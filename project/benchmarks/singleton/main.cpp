#include "singleton.hpp"
#include <benchmark/benchmark.h>
#include <chrono>
#include <future>
#include <iostream>
#include <string>
class SingletonDemo : public Singleton<SingletonDemo>
{
};
static void ThreadBarWithMutex()
{
  auto singleton = SingletonDemo::GetInstance("Bar");
}
static void ThreadBarWithFlag()
{
  auto singleton = SingletonDemo::get("Bar");
}
static void BM_SingletonWithMutex(benchmark::State &state)
{
  for (auto _ : state) { ThreadBarWithMutex(); }
}
// Register the function as a benchmark
BENCHMARK(BM_SingletonWithMutex)->Range(1 << 0, 1 << 10);

// Define another benchmark
static void BM_SingletonWithFlag(benchmark::State &state)
{
  for (auto _ : state) ThreadBarWithFlag();
}
BENCHMARK(BM_SingletonWithFlag)->Range(1 << 0, 1 << 10);

BENCHMARK_MAIN();
