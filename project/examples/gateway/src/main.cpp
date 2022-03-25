
#include "client.hpp"
#include "server.hpp"
#include <cassert>
#include <chrono>
#include <cxxopts.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <uvw.hpp>

int main(int argc, char **argv)
{
  cxxopts::Options options("test", "A brief description");
  // clang-format off
  options
    .set_width(70)
    .set_tab_expansion()
    .allow_unrecognised_options()
    .add_options()
      ("b,bar", "Param bar", cxxopts::value<std::string>())
      ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
      ("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
      ("h,help", "Print usage");
  // clang-format on
  auto  result    = options.parse(argc, argv);
  auto &unmatched = result.unmatched();// get the unmatched arguments
  if (unmatched.size() > 0) {
    std::cout << "unmatched: " << unmatched.size() << std::endl;
    for (auto &u : unmatched) { std::cout << u << std::endl; }
  }
  if (result.count("help") != 0U) {
    std::cout << options.help() << std::endl;
    return 0;
  }
  bool debug = result["debug"].as<bool>();
  if (debug) { std::cout << "Debug mode is enabled" << std::endl; }
  std::string bar;
  if (result.count("bar") != 0U) {
    bar = result["bar"].as<std::string>();
    std::cout << "bar: " << bar << std::endl;
  }

  int foo = result["foo"].as<int>();
  std::cout << "foo: " << foo << std::endl;


  auto loop = uvw::Loop::getDefault();
  int  port = 4242;

  std::string ip     = "127.0.0.1";
  raw_server  server = raw_server(loop);
  raw_client  client = raw_client(loop);
  server.listen(ip, port);
  client.conn(ip,port);
  loop->run();
  loop = nullptr;
  Sleep(1000);
}
