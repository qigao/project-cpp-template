set(TARGET_NAME http_server)
set(TARGET_NAME_LIB ${TARGET_NAME}_lib)
# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Support IDE Visualization
# ##############################################################################

file(GLOB TARGET_SOURCES src/*.cpp src/http/*.cpp src/config/*.cpp)
file(GLOB TARGET_HEADERS include/http/*.hpp include/config/*.hpp)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files"
  FILES ${TARGET_SOURCES}
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files"
  FILES ${TARGET_HEADERS}
)
# ----------------------------------------------------------------------------------------------------------------------
# Find dependencies:
#set(OS_WINDOWS TRUE)
if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()

find_package(Threads REQUIRED)
find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")

# ----------------------------------------------------------------------------------------------------------------------
# Libary part
add_library(${TARGET_NAME_LIB})
target_sources(${TARGET_NAME_LIB} PRIVATE ${TARGET_HEADERS} ${TARGET_SOURCES})
target_include_interface_directories(
  ${TARGET_NAME_LIB} ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/core/shared/utils ${CMAKE_SOURCE_DIR}/core/shared/http
)
# Link dependencies:
target_find_dependencies(
  ${TARGET_NAME_LIB}
  PUBLIC_CONFIG
  httplib
  spdlog
  yyjson
  yaml-cpp
  CLI11
  fmt
)
target_link_system_libraries(
  ${TARGET_NAME_LIB}
  PUBLIC
  httplib::httplib
  spdlog::spdlog
  Threads::Threads
  yyjson::yyjson
  yaml-cpp::yaml-cpp
  hash_lib
  CLI11::CLI11
  fmt::fmt
  $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
  $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)
target_compile_definitions(
  ${TARGET_NAME_LIB}
  INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# link project_options/warnings
target_link_libraries(
  ${TARGET_NAME_LIB} PRIVATE starter_project_options starter_project_warnings
)

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME_LIB}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
          # Any other extra include required
          ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}
)

# Executable part
add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE src/main.cpp)
target_link_libraries(
  ${TARGET_NAME} PRIVATE ${TARGET_NAME_LIB} starter_project_options
                         starter_project_warnings
)
# ----------------------------------------------------------------------------------------------------------------------
# install
add_custom_command(
  TARGET ${TARGET_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/generate_keys.sh
          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
  COMMENT "Copying cmake file as demo to build dir"
)
# ----------------------------------------------------------------------------------------------------------------------
# tests
if(FEATURE_TESTS)
  add_subdirectory(test)
endif()
