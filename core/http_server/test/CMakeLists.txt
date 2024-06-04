# test dependencies
set(PROJECT_NAME http_server_tests)
# ----------------------------------------------------------------------------------------------------------------------
# test the executable part
add_test(
  # calling http_server executable directly
  NAME http_server_test
  COMMAND http_server ""
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------------------------------------------------
# test the library part
add_executable(http_server_lib_tests "./tests.cpp")
target_link_libraries(
  http_server_lib_tests
  PRIVATE my_project_warnings
          my_project_options
          catch2_test_common
          http_server_lib)
# Find dependencies:
target_find_dependencies(http_server_lib_tests PRIVATE_CONFIG fmt)

# Link dependencies:
target_link_system_libraries(http_server_lib_tests PRIVATE fmt::fmt)
# use xml reporter if coverage is enabled
if(${ENABLE_COVERAGE})
  set(COVERAGE_ARGS REPORTER xml)
endif()

# automatically discover tests that are defined in catch based test files you can modify the tests
catch_discover_tests(http_server_lib_tests ${COVERAGE_ARGS})
