#include "hellolib.h"

#include <iostream>

#ifdef WITH_OPENSSL
#include <openssl/sha.h>

#include <array>
#include <iomanip>
#include <sstream>
#endif

using namespace hello;

int32_t hellolib::saySomething(const std::string &something) const noexcept
{
  if (something.empty()) {
    std::cerr << "No value passed\n";
    return 1;
  }

  std::cout << something << '\n';
  return 0;
}

#ifdef WITH_OPENSSL
int32_t hellolib::saySomethingHashed(const std::string &something) const noexcept
{
  if (something.empty()) {
    std::cerr << "No value passed\n";
    return 1;
  }

  std::array<unsigned char, 32> buffer{};

  auto p = SHA256((unsigned char *)something.c_str(), something.size(), buffer.data());
  if (p == nullptr) {
    std::cerr << "SHA256 failed\n";
    return 1;
  }
  // Transform byte-array to string
  std::stringstream shastr;
  shastr << std::hex << std::setfill('0');
  for (const auto &byte : buffer) { shastr << std::setw(2) << (int)byte; }

  std::cout << shastr.str() << '\n' << "\n";
  return 0;
}
#endif
