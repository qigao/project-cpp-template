
#include "hellolib.h"
#include <iostream>
using namespace hello;
#include "baselib.h"
int main()
{



  hellolib hello{};
  int32_t error_code = hello.saySomething("Hello Modern C++ Development");
  if (error_code > 0) { return error_code; }
#ifdef WITH_OPENSSL
  error_code = hello.saySomethingHashed("Hello Modern C++ Development");
  if (error_code > 0) { return error_code; }
#endif
  baselib::printInfo();
  return 0;
}
