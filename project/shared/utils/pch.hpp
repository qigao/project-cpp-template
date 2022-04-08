#ifndef __PCH_H__
#define __PCH_H__



//!Header files for the C++ standard library and extensions, by category.

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

//!C++ Style
#include <stdexcept>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <string_view>
#include <stack>

//!Algorithms
#include <algorithm>
#include <cstdlib>
#include <numeric>


/*C++ Standard Library Containers*/

//!Sequence containers
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>

//!Ordered associative containers
#include <map>
#include <set>

//! Unordered associative containers
#include <unordered_map>
#include <unordered_set>

//!Container adaptors
#include <queue>
#include <stack>

//!Errors and exception handling
#include <cassert>
#include <exception>
#include <stdexcept>
#include <system_error>

//!General utilities
#include <any>

#include <bitset>
#include <cstdlib>


#include <functional>
#include <memory>


#include <optional>
#include <ratio>
#include <scoped_allocator>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <variant>

//!Multithreading
#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>

//!I/O and formatting
#include <cinttypes>
#include <cstdio>


#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>


//!Iterators
#include <iterator>

//!Language support
#include <cfloat>
#include <climits>
#include <codecvt>


#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <limits>
#include <new>
#include <typeinfo>


//!Regular expressions
#include <regex>

//!Strings and character data
#include <charconv>
#include <cctype>
#include <cstdlib>
#include <cstring>


#include <cwchar>
#include <cwctype>
#include <regex>
#include <string>
#include <string_view>

//!Time
#include <chrono>
#include <ctime>

//!C-style Under C++
#include <cassert>
#include <ccomplex>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctgmath>
#include <ctime>
#include <cwchar>
#include <cwctype>

//!Localization
#include <clocale>
#include <codecvt>
#include <locale>

//!Math and numerics

#include <cfenv>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <limits>
#include <numeric>
#include <random>
#include <ratio>
#include <valarray>

//!Memory management
#ifdef __has_include
# if __has_include(<allocators>)
#   include <allocators>
# endif
#endif
#include <memory>
#ifdef __has_include
# if __has_include(<memory_resource>)
#   include <memory_resource>
# endif
#endif
#include <new>
#include <scoped_allocator>

#ifdef _MSC_VER
#include <intrin.h>
#endif


#endif // __PCH_H__
