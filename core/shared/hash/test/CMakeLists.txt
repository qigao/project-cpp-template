find_package(GTest REQUIRED)

# --------------- YAML_CONVERT TEST -------------------
add_executable(hash_function_test hash_function_test.cpp)
target_link_libraries(
  hash_function_test
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          GTest::gmock_main
          hash_lib
          starter_project_options
          starter_project_warnings
)

gtest_discover_tests(hash_function_test)
if(${ENABLE_COVERAGE})
  set(COVERAGE_ARGS REPORTER xml)
endif()
