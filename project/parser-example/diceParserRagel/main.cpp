#include "diceparser.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <sstream>

static void form(benchmark::State &state)
{
  std::string enteredFormula = "(3d6! + 5)*2 + (1d6)d3M1";
  for (auto _ : state) {
    DP::RollFormula   roller;
    std::stringstream sstr(enteredFormula);
    try {
      sstr >> roller;
    } catch (std::runtime_error &e) {
      std::cerr << e.what() << '\n';
    }
    try {
      roller.evaluate();
    } catch (std::runtime_error &e) {
      std::cerr << e.what() << '\n';
    }
  }
}
BENCHMARK(form)->Range(1 << 0, 1 << 10);

BENCHMARK_MAIN();
