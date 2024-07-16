find_package(nanobench)

# ---------------- Singleton Benchmark TEST ---------------------
add_executable(singleton_test singleton_test.cpp)
target_link_libraries(
  singleton_test PRIVATE nanobench::nanobench catch2_test_common
)

catch_discover_tests(
  singleton_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# ----------------- human readable test -------------------------
add_executable(hr_test hr_test.cpp)
target_include_interface_directories(hr_test include ../) # Includes

target_link_libraries(hr_test PRIVATE nanobench::nanobench catch2_test_common)
# include all the necessary directories
target_include_directories(
  hr_test PRIVATE {CMAKE_CURRENT_SOURCE_DIR} # Any other extra include required
                  ../human_readable.hpp
)
catch_discover_tests(hr_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------- random test -------------------------
add_executable(random_test random_test.cpp)
target_sources(random_test PRIVATE ../random.hpp)
target_include_interface_directories(random_test include ../) # Includes

target_link_libraries(
  random_test PRIVATE nanobench::nanobench catch2_test_common
)

catch_discover_tests(random_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------- random test -------------------------
add_executable(function_test auto_function_test.cpp)
target_sources(function_test PRIVATE ../auto_function.hpp)
target_include_interface_directories(function_test include ../) # Includes

target_link_libraries(
  function_test PRIVATE nanobench::nanobench catch2_test_common
)

catch_discover_tests(
  function_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
