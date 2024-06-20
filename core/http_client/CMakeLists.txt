# TargetName
set(TARGET_LIB http_client_lib)
# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES private/src/*.cpp)
file(GLOB TARGET_HEADERS private/include/*.hpp)
file(GLOB TARGET_PUBLIC_HEADERS public/*.h)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files"
  FILES ${TARGET_SOURCES}
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header(Private) Files"
  FILES ${TARGET_HEADERS}
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header(Public) Files"
  FILES ${TARGET_PUBLIC_HEADERS}
)

# ----------------------------------------------------------------------------------------------------------------------
# Find dependencies:

if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()

# ----------------------------------------------------------------------------------------------------------------------
# library part
add_library(${TARGET_LIB})
target_sources(
  ${TARGET_LIB} PRIVATE ${TARGET_HEADERS} ${TARGET_SOURCES}
                        ${TARGET_PUBLIC_HEADERS}
)
target_include_interface_directories(
  ${TARGET_LIB} ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/core/shared/utils ${CMAKE_SOURCE_DIR}/core/shared/http
)
find_package(Threads)
# Link dependencies:
target_find_dependencies(${TARGET_LIB} PUBLIC_CONFIG httplib spdlog cpp_yyjson)
target_link_system_libraries(
  ${TARGET_LIB}
  PUBLIC
  httplib::httplib
  spdlog::spdlog
  Threads::Threads
  cpp_yyjson::cpp_yyjson
  $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
  $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)

set_target_properties(
  ${TARGET_LIB} PROPERTIES CXX_VISIBILITY_PRESET hidden
                           VISIBILITY_INLINES_HIDDEN ON
)
target_compile_definitions(${TARGET_LIB} PRIVATE ${TARGET_LIB}_EXPORTS)
target_compile_definitions(
  ${TARGET_LIB}
  INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# link project_options/warnings
target_link_libraries(
  ${TARGET_LIB} PRIVATE starter_project_options starter_project_warnings
)
# include all the necessary directories
target_include_directories(
  ${TARGET_LIB}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/include
          ${CMAKE_CURRENT_SOURCE_DIR}/public
          # Any other extra include required
          ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS}
)

# Tests
if(FEATURE_TESTS)
  add_subdirectory(test)
endif()

# =================================================================================
# INSTALL
# =================================================================================

# Import GNU Install Rules
include(GNUInstallDirs)

# Files to Install
install(
  TARGETS ${TARGET_LIB}
  EXPORT ${TARGET_LIB}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_LIB}
)

install(
  EXPORT ${TARGET_LIB}Targets
  FILE ${TARGET_LIB}Targets.cmake
  NAMESPACE ${TARGET_LIB}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_LIB}
)
