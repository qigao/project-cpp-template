find_package(GTest REQUIRED)
add_executable(http_client_test http_client_test.cpp)
target_link_libraries(
  http_client_test
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          GTest::gmock_main
          http_server_lib
          http_client_lib
          starter_project_options
          starter_project_warnings
)

gtest_discover_tests(http_client_test)

add_executable(sum_test sum_test.cpp)
target_link_libraries(
  sum_test
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          GTest::gmock_main
          http_server_lib
          http_client_lib
          starter_project_options
          starter_project_warnings
)
gtest_discover_tests(sum_test)

include(CodeCoverage)
append_coverage_compiler_flags()

setup_target_for_coverage_gcovr_html(
  NAME
  sum_test_coverage
  EXECUTABLE
  sum_test
  BASE_DIRECTORY
  ../
)

setup_target_for_coverage_gcovr_html(
  NAME
  http_client_coverage
  EXECUTABLE
  http_client_test
  BASE_DIRECTORY
  ../
)
