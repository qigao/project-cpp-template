find_package(GTest REQUIRED)
add_executable(yaml_convert_test yaml_convert_test.cpp)
target_link_libraries(
  yaml_convert_test
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          GTest::gmock_main
          http_server_lib
          starter_project_options
          starter_project_warnings
)

gtest_discover_tests(yaml_convert_test)

add_executable(http_server_test http_server_test.cpp)
target_link_libraries(
  http_server_test
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          GTest::gmock_main
          http_server_lib
          starter_project_options
          starter_project_warnings
)
gtest_discover_tests(http_server_test)

include(CodeCoverage)
append_coverage_compiler_flags()

setup_target_for_coverage_gcovr_html(
  NAME
  yaml_convert_coverage
  EXECUTABLE
  yaml_convert_test
  BASE_DIRECTORY
  ../
)

setup_target_for_coverage_gcovr_html(
  NAME
  http_server_coverage
  EXECUTABLE
  http_server_test
  BASE_DIRECTORY
  ../
)
