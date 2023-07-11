#include <benchmark/benchmark.h>
#include <string>
#include "URIParser.h"
#include "uri_values.h"
using namespace Parser;

static void sipURI(benchmark::State& state) {
  auto sip_uri = std::string(
      "sips:alice:secretW0rd@gateway.com:5061;transport=udp;user=phone;method="
      "REGISTER?subject=sales%20meeting&priority=urgent&to=sales%40city.com");
  for (auto _ : state) {
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);
  }
}

BENCHMARK(sipURI)->Range(1 << 0, 1 << 10);

static void httpURI(benchmark::State& state) {
  auto http_uri = std::string("http://example.com:81/a/b/c?x=7&y=11");
  for (auto _ : state) {
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    parser->execute(http_uri.data(), http_uri.length(), 0, values);
  }
}

BENCHMARK(httpURI)->Range(1 << 0, 1 << 10);

BENCHMARK_MAIN();
