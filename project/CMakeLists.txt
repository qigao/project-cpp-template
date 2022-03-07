# MIT License
# Copyright (c) 2018-Today Michele Adduci <adduci@tutanota.com>
#
# Project-related instructions

# Activate Testing, if given
if(BUILD_TESTING)
  enable_testing()
endif()
# Define the two required variables before including
# the source code for watching a git repository.
set(PRE_CONFIGURE_FILE "git.h.in")
set(POST_CONFIGURE_FILE "${CMAKE_BINARY_DIR}/git.h")
include(${CMAKE_SOURCE_DIR}/cmake/git.cmake)
#####################################
# Define Targets
#####################################
add_subdirectory(hellolib)
add_subdirectory(helloapp)
add_subdirectory(baselib)
#####################################
# Define Install Targets
#####################################
install(TARGETS
  hellolib
  helloapp
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
  ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
  PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include
)

install(
  FILES
    ${CMAKE_SOURCE_DIR}/project/hellolib/include/hellolib.h
    ${CMAKE_BINARY_DIR}/exports/hellolib_export.h
  DESTINATION
    include/hellolib)

install(FILES
  ${CMAKE_SOURCE_DIR}/LICENSE
DESTINATION .)
