# Required for Testing
if(BUILD_TESTING)
  find_package(doctest REQUIRED)
endif()
if (BUILD_BENCHMARKS)
  find_package(benchmark REQUIRED)
endif ()
# Optional Dependency, doesn't trigger error if missing
find_package(OpenSSL)
find_package(cxxopts)
