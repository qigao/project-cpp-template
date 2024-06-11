set(PROJECT_NAME http_server)
# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES src/*.cpp src/http/*.cpp)
file(GLOB TARGET_HEADERS include/helper/*.hpp include/http/*.hpp)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${TARGET_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${TARGET_HEADERS})
# ----------------------------------------------------------------------------------------------------------------------
# Find dependencies:
#set(OS_WINDOWS TRUE)
if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()

find_package(httplib)
find_package(RapidJSON REQUIRED)
find_package(cpp_yyjson REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(ryml REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")

# ----------------------------------------------------------------------------------------------------------------------
# executable part
add_executable(${PROJECT_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES})
# Link dependencies:
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE httplib::httplib
          spdlog::spdlog_header_only
          Threads::Threads
          rapidjson
          cpp_yyjson::cpp_yyjson
          ryml::ryml
          $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
          $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)

target_compile_definitions(
  ${PROJECT_NAME} INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# link project_options/warnings
target_link_libraries(${PROJECT_NAME} PRIVATE my_project_options my_project_warnings)

# include all the necessary directories
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
          # Any other extra include required
          ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}
)
# ----------------------------------------------------------------------------------------------------------------------
# install
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/generate_keys.sh
          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
  COMMENT "Copying cmake file as demo to build dir"
)
# ----------------------------------------------------------------------------------------------------------------------
# tests
if(FEATURE_TESTS)
  add_subdirectory("./test")
endif()
