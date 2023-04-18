# Required for Testing
if(BUILD_TESTING)
  find_package(GTest REQUIRED)
endif()
find_package(benchmark)

# Optional Dependency, doesn't trigger error if missing
find_package(cxxopts)
find_package(Threads REQUIRED)
find_package(readerwriterqueue REQUIRED)
find_package(concurrentqueue REQUIRED)
