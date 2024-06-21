find_package(GTest REQUIRED)
include(CodeCoverage)

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

# --------------- COVERAGE -----------------------------

append_coverage_compiler_flags()
setup_target_for_coverage_gcovr_html(
  NAME
  hash_function_coverage
  EXECUTABLE
  hash_function_test
  BASE_DIRECTORY
  ${PROJECT_SOURCE_DIR}/core/shared/hash
)
