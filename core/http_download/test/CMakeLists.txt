# test dependencies

# ----------------------------------------------------------------------------------------------------------------------
# test the executable part
add_test(# calling http_download_exe executable directly
         NAME http_download_exe_test COMMAND http_download_exe ""
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# ----------------------------------------------------------------------------------------------------------------------
# test the library part
add_executable(http_download_exe_lib_tests "main.cpp")
target_link_libraries(
  http_download_exe_lib_tests PRIVATE my_project_warnings my_project_options catch2_test_common
                                      http_download_exe_lib
)

# use xml reporter if coverage is enabled
if(${ENABLE_COVERAGE})
  set(COVERAGE_ARGS REPORTER xml)
endif()

# automatically discover tests that are defined in catch based test files you can modify the tests
catch_discover_tests(http_download_exe_lib_tests ${COVERAGE_ARGS})
