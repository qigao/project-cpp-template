# Separate library part and executable part (`main()` function) to simplify testing.
set(PROJECT_NAME http_server)
set(PROJECT_LIB http_server_lib)
# ----------------------------------------------------------------------------------------------------------------------
# library part
add_library(${PROJECT_LIB} INTERFACE) # use interface or normal library according to the implementation
# link project_options/warnings
target_link_libraries(${PROJECT_LIB} INTERFACE my_project_options my_project_warnings)

if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()
#set(OS_WINDOWS TRUE)

# Includes
target_include_interface_directories(${PROJECT_LIB} include)

# Find dependencies:
target_find_dependencies(
  ${PROJECT_LIB}
  INTERFACE_CONFIG
  fmt
  httplib
  nlohmann_json)

# Link dependencies:
target_link_system_libraries(
  ${PROJECT_LIB}
  INTERFACE
  fmt::fmt
  httplib::httplib
  nlohmann_json::nlohmann_json)

# ----------------------------------------------------------------------------------------------------------------------
# executable part
add_executable(
  ${PROJECT_NAME} "./src/main.cpp" include/http_server/http_server.hpp include/my_exe/human_readable.hpp
        include/my_exe/constants.hpp
)# just for `main.cpp` which contains `main()` function
# link project_options/warnings
target_link_libraries(${PROJECT_NAME} PRIVATE my_project_options my_project_warnings)

# Includes, just link the library part
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB})

target_link_libraries(${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
                                             $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>)

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:HTTP_OPENSSL_SUPPORT>)
## ----------------------------------------------------------------------------------------------------------------------
# tests
if(FEATURE_TESTS)
  add_subdirectory("./test")
endif()
