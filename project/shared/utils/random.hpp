#ifndef __RANDOM_H__
#define __RANDOM_H__
#include <string>
#include <random>
std::string random_string(std::string::size_type length)
{
  static auto &chrs =
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  thread_local static std::mt19937 rg{ std::random_device{}() };
  thread_local static std::uniform_int_distribution<std::string::size_type> pick(
    0, std::size(chrs) - 2);

  std::string s;
  s.reserve(length);
  while (length--) s += chrs[pick(rg)];
  return s;
}
#endif // __RANDOM_H__
