# Required for Testing
if(BUILD_TESTING)
  find_package(Gtest REQUIRED)
endif()

if(BUILD_BENCHMARKS)
  find_package(benchmark REQUIRED)
endif()

# Optional Dependency, doesn't trigger error if missing
find_package(OpenSSL)
find_package(cxxopts)
find_package(Threads REQUIRED)
find_package(uvw CONFIG REQUIRED)
find_package(readerwriterqueue REQUIRED)
find_package(concurrentqueue REQUIRED)
