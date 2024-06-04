set(TARGET_NAME http_server)
set(TARGET_NAME_LIB ${TARGET_NAME}_lib)
# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Support IDE Visualization
# ##############################################################################

file(GLOB TARGET_SOURCES src/http/*.cpp src/config/*.cpp)
file(GLOB TARGET_HEADERS include/http/*.hpp include/config/*.hpp include/*.h)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files"
  FILES ${TARGET_SOURCES}
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files"
  FILES ${TARGET_HEADERS}
)

# ----------------------------------------------------------------------------------------------------------------------
# Libary part
add_library(${TARGET_NAME_LIB} INTERFACE)
target_sources(${TARGET_NAME_LIB} INTERFACE ${TARGET_HEADERS} ${TARGET_SOURCES})
target_include_interface_directories(
  ${TARGET_NAME_LIB} ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/core/shared/utils ${CMAKE_SOURCE_DIR}/core/shared/http
)
# ----------------------------------------------------------------------------------------------------------------------
# Find dependencies:
if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()

find_package(Threads REQUIRED)
# Link dependencies:
target_find_dependencies(
  ${TARGET_NAME_LIB}
  INTERFACE_CONFIG
  httplib
  spdlog
  yyjson
  yaml-cpp
  cxxopts
  bshoshany-thread-pool
)
target_link_system_libraries(
  ${TARGET_NAME_LIB}
  INTERFACE
  httplib::httplib
  spdlog::spdlog_header_only
  Threads::Threads
  yyjson::yyjson
  yaml-cpp::yaml-cpp
  hashfunc
  cxxopts::cxxopts
  bshoshany-thread-pool::bshoshany-thread-pool
  $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
  $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)
target_compile_definitions(
  ${TARGET_NAME_LIB}
  INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# link project_options/warnings
target_link_libraries(
  ${TARGET_NAME_LIB} INTERFACE starter_project_options starter_project_warnings
)
target_precompile_headers(
  ${TARGET_NAME_LIB} INTERFACE include/config/pch_headers.hpp
)
# Executable part
add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE src/main.cpp)
target_link_libraries(
  ${TARGET_NAME} PRIVATE ${TARGET_NAME_LIB} starter_project_options
                         starter_project_warnings
)
add_custom_command(
  TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/application.yml
          $<TARGET_FILE_DIR:${TARGET_NAME}>/application.yml
)
# tests
if(FEATURE_TESTS)
  add_subdirectory(test)
endif()
