find_package(GTest REQUIRED)
add_executable(http_client_test http_client_test.cpp)
target_link_libraries(
  http_client_test
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          GTest::gmock_main
          http_server_lib
          http_client
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
          http_client
          starter_project_options
          starter_project_warnings
)
gtest_discover_tests(sum_test)
if(${ENABLE_COVERAGE})
  set(COVERAGE_ARGS REPORTER xml)
endif()
