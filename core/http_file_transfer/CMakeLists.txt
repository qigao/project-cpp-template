# =================================================================================
# PROJECT
# =================================================================================

# Project Name
set(PROJECT_NAME http_file_transfer)

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES private/src/*.cpp)
file(GLOB TARGET_HEADERS private/include/*.hpp)
file(GLOB TARGET_PUBLIC_HEADERS public/*.h)
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
add_library(${PROJECT_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES} ${TARGET_PUBLIC_HEADERS})
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
set_target_properties(
  ${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN ON
)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_EXPORTS)
target_compile_definitions(
  ${PROJECT_NAME} INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# link project_options/warnings
target_compile_options(${PROJECT_NAME} PRIVATE my_project_options my_project_warnings)
# include all the necessary directories
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/include ${CMAKE_CURRENT_SOURCE_DIR}/public
          # Any other extra include required
)
# =================================================================================
# INSTALL
# =================================================================================

# Import GNU Install Rules
include(GNUInstallDirs)

# Files to Install
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
