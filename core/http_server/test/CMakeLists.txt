find_package(GTest REQUIRED)
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
