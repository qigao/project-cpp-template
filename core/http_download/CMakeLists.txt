# Separate library part and executable part (`main()` function) to simplify testing.
set(PROJECT_NAME http_download)
# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES src/*.cpp)
file(GLOB TARGET_HEADERS include/http/*.hpp)

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
find_package(fmt)
find_package(Threads REQUIRED)
find_package(spdlog)
# ----------------------------------------------------------------------------------------------------------------------
# executable part
add_executable(${PROJECT_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES})
# Link dependencies:
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC fmt::fmt
         httplib::httplib
         spdlog::spdlog
         Threads::Threads
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
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                          # Any other extra include required
)
# ----------------------------------------------------------------------------------------------------------------------
# install
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
  COMMENT "Copying cmake file as demo to build dir"
)
# ----------------------------------------------------------------------------------------------------------------------
# tests
if(FEATURE_TESTS)
  add_subdirectory("./test")
endif()
