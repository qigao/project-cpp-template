# test dependencies
set(PROJECT_NAME "http_client_lib_test")
# test executable
add_executable(${PROJECT_NAME} "./tests.cpp")
# Find dependencies:
if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()

#set(OS_WINDOWS TRUE)
find_package(httplib CONFIG REQUIRED)
# Link dependencies:
target_link_system_libraries(
  ${PROJECT_NAME}
  PRIVATE
  fmt::fmt
  httplib::httplib)
target_link_libraries(${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
                                             $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>)

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE http_client_lib
          my_project_warnings
          my_project_options
          catch2_test_common)

# use xml reporter if coverage is enabled
if(${ENABLE_COVERAGE})
  set(COVERAGE_ARGS REPORTER xml)
endif()

# automatically discover tests that are defined in catch based test files you can modify the tests
catch_discover_tests(${PROJECT_NAME} ${COVERAGE_ARGS})

# constexpr tests
add_subdirectory("./constexpr")
