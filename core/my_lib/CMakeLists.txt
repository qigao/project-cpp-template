# =================================================================================
# PROJECT
# =================================================================================

# Project Name
set(PROJECT_NAME "mylib")

# =================================================================================
# CONFIG
# =================================================================================

# =================================================================================
# HEADERS
# =================================================================================

set(MY_LIB_HEADERS #
    "public/mylib.h"
)

# Headers used to access library.
set(PUBLIC_HEADERS "public/mylib.h")

# =================================================================================
# SOURCES
# =================================================================================

set(MY_LIB_SOURCES #
    "private/mylib.cpp"
)

# =================================================================================
# BUILD
# =================================================================================

# Create SHARED Library Object
add_library(mylib SHARED ${MY_LIB_SOURCES})
# link project_options/warnings
target_link_libraries(mylib PRIVATE my_project_options my_project_warnings)

# Configure SHARED Library Object
set_target_properties(
  mylib PROPERTIES PUBLIC_HEADER ${PUBLIC_HEADERS} RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
                   VERSION ${PROJECT_VERSION}
)
set_target_properties(mylib PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN ON)
target_compile_definitions(mylib PRIVATE MYLIB_EXPORTS)
# Find dependencies:
find_package(fmt)

# Link dependencies:
target_link_system_libraries(mylib PRIVATE fmt::fmt)
# Includes
target_include_interface_directories(mylib public)
# =================================================================================
# INSTALL
# =================================================================================

# Import GNU Install Rules
include(GNUInstallDirs)

# Files to Install
install(
  TARGETS mylib
  EXPORT mylibTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mylib
)

install(
  EXPORT mylibTargets
  FILE mylibTargets.cmake
  NAMESPACE mylib::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mylib
)
