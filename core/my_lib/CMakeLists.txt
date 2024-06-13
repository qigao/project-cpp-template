# =================================================================================
# PROJECT
# =================================================================================

# Project Name
set(PROJECT_NAME my_lib)

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES private/*.cpp)
file(GLOB TARGET_HEADERS private/*.h)

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

# Find dependencies:
find_package(fmt)

# executable part
add_library(${PROJECT_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES})

# Link dependencies:
target_link_system_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

# Include all necessary directories
target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private
                          ${CMAKE_CURRENT_SOURCE_DIR}/public
  # Any other extra include required
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden
                             VISIBILITY_INLINES_HIDDEN ON
)
# link project_options/warnings
target_link_libraries(
  ${PROJECT_NAME} PRIVATE starter_project_options starter_project_warnings
)

# Add the compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE MYLIB_EXPORTS)

# =================================================================================
# INSTALL
# =================================================================================

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
