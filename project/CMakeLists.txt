
# Activate Testing, if given
if(BUILD_TESTING)
  enable_testing()
endif()
# Define the two required variables before including the source code for
# watching a git repository.
set(PRE_CONFIGURE_FILE "git.h.in")
set(POST_CONFIGURE_FILE "${CMAKE_BINARY_DIR}/git.h")
include(${CMAKE_SOURCE_DIR}/cmake/git.cmake)

set(CMAKE_PREFIX_PATH "d:\\Qt\\6.2.3\\msvc2019_64\\")

add_subdirectory(examples)
