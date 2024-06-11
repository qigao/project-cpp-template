set(PROJECT_NAME http_server_tests)
# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES ../src/http/*.cpp)
file(GLOB TARGET_HEADERS ../include/helper/*.hpp ../include/http/*.hpp)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/.. PREFIX "Source Files" FILES ${TARGET_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/.. PREFIX "Header Files" FILES ${TARGET_HEADERS})
# ----------------------------------------------------------------------------------------------------------------------
# Find dependencies:
#set(OS_WINDOWS TRUE)
if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()

find_package(httplib)
find_package(cpp_yyjson REQUIRED)
find_package(ryml REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")

# ----------------------------------------------------------------------------------------------------------------------
# executable part
add_executable(${PROJECT_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES} tests.cpp)
# Link dependencies:
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE httplib::httplib
          spdlog::spdlog_header_only
          Threads::Threads
          cpp_yyjson::cpp_yyjson
          ryml::ryml
          $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
          $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)

target_compile_definitions(
  ${PROJECT_NAME} INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# link project_options/warnings
target_link_libraries(
  ${PROJECT_NAME} PRIVATE my_project_options my_project_warnings catch2_test_common
)

# include all the necessary directories
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
          # Any other extra include required
          ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}
)
add_test(NAME MyTest COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})
