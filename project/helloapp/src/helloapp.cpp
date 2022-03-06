#include "git.h"
#include "hellolib.h"
#include <iostream>
using namespace hello;

int main()
{

  std::cout << "Version: " << PROJECT_VERSION << "\n"
            << "Commit: " << GIT_DESCRIBE << "\n"
            << "Author: " << GIT_AUTHOR_NAME << " <" << GIT_AUTHOR_EMAIL << ">\n"
            << "State: " << (GIT_IS_DIRTY ? "dirty" : "") << "\n"
            << "Date: " << GIT_COMMIT_DATE_ISO8601 << "\n\n"
            << GIT_COMMIT_SUBJECT << "\n"
            << GIT_COMMIT_BODY << std::endl;

  hellolib hello{};
  int32_t error_code = hello.saySomething("Hello Modern C++ Development");
  if (error_code > 0) { return error_code; }
#ifdef WITH_OPENSSL
  error_code = hello.saySomethingHashed("Hello Modern C++ Development");
  if (error_code > 0) { return error_code; }
#endif
  return 0;
}
