#include <string>

#include "hellolib_export.h"

namespace hello {
class HELLOLIB_EXPORT hellolib
{
public:
  int32_t saySomething(const std::string &something) const noexcept;
#ifdef WITH_OPENSSL
  int32_t saySomethingHashed(const std::string &something) const noexcept;
#endif
};
}// namespace hello
