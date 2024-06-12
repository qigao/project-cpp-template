# =================================================================================
# PROJECT
# =================================================================================

# Project Name
set(PROJECT_NAME "myExe")

# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES *.cpp)
file(GLOB TARGET_HEADERS *.hpp)

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
find_package(cpp_yyjson REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(ryml REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# ----------------------------------------------------------------------------------------------------------------------
# executable part
add_executable(${PROJECT_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES})
# Link dependencies:
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE httplib::httplib
          spdlog::spdlog_header_only
          Threads::Threads
          cpp_yyjson::cpp_yyjson
          ryml::ryml
          my_lib
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
          ${PROJECT_SOURCE_DIR}/core/my_lib/public
)
# ----------------------------------------------------------------------------------------------------------------------
# install
# Files to Install
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
