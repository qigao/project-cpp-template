# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME singleton_benchmark)
find_package(nanobench)

# ---------------- Singleton Benchmark TEST ---------------------
add_executable(singleton_benchmark singleton_test.cpp)
target_link_libraries(
  singleton_benchmark PRIVATE nanobench::nanobench catch2_test_common
)
catch_discover_tests(
  singleton_benchmark WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
