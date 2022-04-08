#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <uvw.hpp>
#include <uvw/util.h>
auto get_interface_list()
{
  std::map<std::string, std::string> if_map;

  auto interfaceAdrs = uvw::Utilities::interfaceAddresses();

  auto it = interfaceAdrs.begin();
  while (it != interfaceAdrs.end()) {
    if ((!it->internal) && (it->address.ip.find('.') != std::string::npos)) {
      if_map.emplace(std::make_pair("ip", it->address.ip));
      if_map.emplace(std::make_pair("mask", it->netmask.ip));
      if_map.emplace(std::make_pair("mac", it->physical));
      if_map.emplace(std::make_pair("name", it->name));
    }
    ++it;
  }
  return if_map;
}

auto get_cpu_info()
{
  auto cpu_info = uvw::Utilities::cpuInfo();

  std::map<std::string, std::string> result{
    { "size", std::to_string(cpu_info.size()) },
    { "model", cpu_info[0].model },
    { "speed", std::to_string(cpu_info[0].speed) }
  };
  return result;
}
auto get_load_average() { return uvw::Utilities::loadAverage(); }
auto get_total_memory() { return uvw::Utilities::totalMemory(); }
auto get_up_time() { return uvw::Utilities::uptime(); }
auto get_hr_time() { return uvw::Utilities::hrtime(); }
auto get_path() { return uvw::Utilities::path(); }
auto get_cwd() { return uvw::Utilities::cwd(); }
auto get_rusage() { return uvw::Utilities::rusage(); }
#endif// __UTILS_H__
