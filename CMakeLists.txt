# ##############################################################################
# Define Project-Wide Settings
# ##############################################################################
cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

# Enables the Visibility Property on all target types
cmake_policy(SET CMP0063 NEW)

# Enables the MSVC_RUNTIME_LIBRARY property on targets
cmake_policy(SET CMP0091 NEW)

# Define the Project Name and Description
project(
  moderncpp-project
  LANGUAGES CXX C
  DESCRIPTION "moderncpp-project for Modern C++")

# Set additional project information
set(CMAKE_PROJECT_VERSION_MAJOR 1)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 0)
set(CMAKE_PROJECT_VERSION
  ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}
)
set(APPLICATION_COMPANY_NAME "DemoApp Research")
set(APPLICATION_COMPANY_DOMAIN "github.com")
set(APPLICATION_WEBSITE "https://github.com/qigao/cmake-template")
set(APPLICATION_COPYRIGHT "Copyright Since 2016, Qi Gao")
set(APPLICATION_LICENSE "MIT License")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Set required C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Generate the compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Set the Relative Path Configurations
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_MACOSX_RPATH OFF)

# Configure the visibility of symbols in targets
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# Define installation folder and the module path (used by conan)
set(PKG_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/pkg)
set(CMAKE_MODULE_PATH ${PKG_INSTALL_PREFIX} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${PKG_INSTALL_PREFIX} ${CMAKE_PREFIX_PATH})

# Define Shared folder for the project
set(SHARED_DIR ${PROJECT_SOURCE_DIR}/project/shared)

# Define binary folder for the project
set(BINARY_INSTALL_DIR ${PROJECT_SOURCE_DIR}/bin)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# ##############################################################################
# Define Options
# ##############################################################################
option(BUILD_TESTING "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_WITH_MT "Build libraries as MultiThreaded DLL (Windows Only)" OFF)
option(BUILD_WITH_DOCS "Build documentation" OFF)
option(BUILD_WITH_QT6 "Build QT6 examples" OFF)
option(BUILD_WITH_IMGUI "Build Imgui examples" OFF)
option(BUILD_BENCHMARKS "Build benchmarks" ON)


# ##############################################################################
# Define CMake Module Imports
# ##############################################################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(${CMAKE_SOURCE_DIR}/cmake/clang_format.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/compiler_options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cpack.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/sanitizers.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/code_coverage.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/ragel.cmake)
# ##############################################################################
# Source Directories
# ##############################################################################
add_subdirectory(project)
