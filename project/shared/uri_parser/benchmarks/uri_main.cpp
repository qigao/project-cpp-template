#include "uri_func.h"
#include "uri_parser.h"
#include <benchmark/benchmark.h>
#include <iostream>
#include <string>

static void sipURI(benchmark::State &state)
{
  NamedValues values;
  uri_parser *parser  = new uri_parser();
  auto        sip_uri = std::string(
    "sips:alice:secretW0rd@gateway.com:5061;transport=udp;user=phone;method="
           "REGISTER?subject=sales%20meeting&priority=urgent&to=sales%40city.com");
  for (auto _ : state) { parser->execute(sip_uri.data(), sip_uri.length() + 1, 0, values); }
  delete parser;
}
BENCHMARK(sipURI)->Range(1 << 0, 1 << 10);

static void httpURI(benchmark::State &state)
{
  NamedValues values;
  uri_parser *parser  = new uri_parser();
  auto        http_uri = std::string(
    "http://example.com:81/a/b/c?x=7&y=11" );
  for (auto _ : state) { parser->execute(http_uri.data(), http_uri.length() + 1, 0, values); }
  delete parser;
}
BENCHMARK(httpURI)->Range(1 << 0, 1 << 10);

BENCHMARK_MAIN();
