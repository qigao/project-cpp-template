# Activate Testing, if given
if (BUILD_TESTING)
  enable_testing()
endif ()

# Define the two required variables before including the source code for
# watching a git repository.
set(PRE_CONFIGURE_FILE "version.h.in")
set(POST_CONFIGURE_FILE "${CMAKE_BINARY_DIR}/version.h")
include(${CMAKE_SOURCE_DIR}/cmake/git_info.cmake)

# ##############################################################################
# Define Targets
# ##############################################################################
if (BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif ()

add_subdirectory(examples)
