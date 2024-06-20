# =================================================================================
# PROJECT
# =================================================================================

# Project Name
set(TARGET_NAME_LIB async_download_lib)
set(TARGET_NAME async_download)
# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES *.cpp)
file(GLOB TARGET_HEADERS *.hpp)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
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

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
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
  ${TARGET_NAME_LIB} PUBLIC_CONFIG httplib spdlog cpp_yyjson
)
target_link_system_libraries(
  ${TARGET_NAME_LIB}
  PUBLIC
  httplib::httplib
  spdlog::spdlog_header_only
  Threads::Threads
  cpp_yyjson::cpp_yyjson
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
target_sources(${TARGET_NAME} PRIVATE main.cpp)
target_link_libraries(
  ${TARGET_NAME} PRIVATE ${TARGET_NAME_LIB} starter_project_options
                         starter_project_warnings
)
# ----------------------------------------------------------------------------------------------------------------------
# install
# Files to Install
install(
  TARGETS ${TARGET_NAME}
  EXPORT ${TARGET_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
  EXPORT ${TARGET_NAME}Targets
  FILE ${TARGET_NAME}Targets.cmake
  NAMESPACE ${TARGET_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
)
