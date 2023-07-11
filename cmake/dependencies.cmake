# Required for Testing
if(BUILD_TESTING)
  find_package(GTest CONFIG)
  find_package(Catch2 3)
endif()
find_package(benchmark)

# Optional Dependency, doesn't trigger error if missing
find_package(cxxopts)
find_package(Threads)
find_package(readerwriterqueue)
find_package(concurrentqueue)
find_package(spdlog)
