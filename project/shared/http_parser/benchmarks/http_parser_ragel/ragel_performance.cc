#include <stdint.h>
#include <string>
#include "defines.h"
#include <benchmark/benchmark.h>

static void Ragel_HTTP(benchmark::State &state){
  http_parser_settings settings_null = {};
  http_parser    parser;
  for (auto _: state) {
     http_parser_init(&parser, HTTP_REQUEST);
     http_parser_execute(&parser, &settings_null, kMessage.c_str(),
        kMessage.size());
  }
}
BENCHMARK(Ragel_HTTP)->Range(1 << 0, 1 << 10);

BENCHMARK_MAIN();
