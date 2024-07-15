cmake_minimum_required(VERSION 3.21)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Define the features of the project
# -------------------------------------------------------------------
include(tailored_options)

# enable cross-compiling: - should be called before run_vcpkg()
if(ENABLE_CROSS_COMPILING)
    enable_cross_compiler()
endif()

# Set additional project information
set(CMAKE_PROJECT_VERSION_MAJOR 1)
set(CMAKE_PROJECT_VERSION_MINOR 1)
set(CMAKE_PROJECT_VERSION_PATCH 3)

set(PROJECT_VERSION
    ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}
)

set(PROJECT_NAME "DemoApp Research")
set(PROJECT_DESCRIPTION "multi-module template project for modern C++")
set(PROJECT_WEBSITE "http://github.com/project-cpp-template.git")
set(APPLICATION_LICENSE "MIT License")

# Set the project name and language
project(
    cpp_core_project
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    HOMEPAGE_URL ${PROJECT_WEBSITE}
    LANGUAGES CXX C
)

## projects
# ----------------------------------------------
# Set the features of the project
include(Config.cmake)

# threads
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}"
                                                     STREQUAL "GNU")
)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(THREADS_PREFER_PTHREAD_FLAG OFF)
endif()
find_package(Threads REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# share vars with Taskfile
file(
    WRITE ${CMAKE_BINARY_DIR}/task_vars.env
    "
    CMAKE_BUILD_DIR=${CMAKE_BINARY_DIR}
    CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    CMAKE_OUTPUT_DIR=${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}
  "
)
file(CREATE_LINK ${CMAKE_BINARY_DIR}/task_vars.env
     ${PROJECT_SOURCE_DIR}/task_vars.env
)
# --------------------------------------------------------------
add_subdirectory(configured_files)

add_subdirectory(core)
if(ENABLE_SAMPLE_PROJECT_COMPILING)
    add_subdirectory(project)
endif()
