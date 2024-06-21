find_package(GTest REQUIRED)
include(CodeCoverage)

# --------------- YAML_CONVERT TEST -------------------
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

# ---------------- HTTP_SERVER TEST ---------------------
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

# --------------- YAML_PROPERTIES ---------------------
add_executable(yaml_properties_test yaml_properties_test.cpp)
target_link_libraries(
  yaml_properties_test
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          GTest::gmock_main
          http_server_lib
          starter_project_options
          starter_project_warnings
)
gtest_discover_tests(yaml_properties_test)

# --------------- COVERAGE -----------------------------

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
setup_target_for_coverage_gcovr_html(
  NAME
  yaml_properties_coverage
  EXECUTABLE
  yaml_properties_test
  BASE_DIRECTORY
  ../
)
