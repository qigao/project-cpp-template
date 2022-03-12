# Activate Testing, if given
if(BUILD_TESTING)
  enable_testing()
endif()
# Define the two required variables before including the source code for
# watching a git repository.
include(${CMAKE_SOURCE_DIR}/cmake/git_info.cmake)
gitinforeport(${PROJECT_SOURCE_DIR})
configure_file(${CMAKE_SOURCE_DIR}/project/version.h.in
               "${PROJECT_BINARY_DIR}/version.h")

# ##############################################################################
# Define Targets
# ##############################################################################

add_subdirectory(examples)
