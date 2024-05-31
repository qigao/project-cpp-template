
#include <cxxopts.hpp>
#include <iostream>

#include "baselib.h"
#include "hellolib.h"

static constexpr const int fixed_option_width = 100;
int main(int argc, char **argv)
{
  cxxopts::Options options("test", "A brief description");
  // clang-format off
  options
    .set_width(fixed_option_width)
    .set_tab_expansion()
    .allow_unrecognised_options()
    .add_options()
      ("b,bar", "Param bar", cxxopts::value<std::string>())
      ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
      ("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
      ("h,help", "Print usage");
  // clang-format on
  auto  result    = options.parse(argc, argv);
  const auto &unmatched = result.unmatched();// get the unmatched arguments
  if (!unmatched.empty()) {
    std::cout << "unmatched: " << unmatched.size() << '\n';
    for (auto &u : unmatched) { std::cout << u << '\n'; }
  }
  if (result.count("help") != 0U) {
    std::cout << options.help() << '\n';
    return 0;
  }
  bool debug = result["debug"].as<bool>();
  if (debug) {
    std::cout << "Debug mode is enabled" << '\n';
    baselib::printInfo();
  }
  std::string bar;
  if (result.count("bar") != 0U) {
    bar = result["bar"].as<std::string>();
    std::cout << "bar: " << bar << '\n';
  }

  int foo = result["foo"].as<int>();
  std::cout << "foo: " << foo << '\n';
  hello::hellolib hello{};
  int32_t  error_code = hello.saySomething("Hello Modern C++ Development");
  if (error_code > 0) { return error_code; }
#ifdef WITH_OPENSSL
  error_code = hello.saySomethingHashed("Hello Modern C++ Development");
  if (error_code > 0) { return error_code; }
#endif
  return 0;
}
