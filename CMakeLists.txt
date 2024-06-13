cmake_minimum_required(VERSION 3.21...3.28)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(fetch_project_options)

# Define the features of the project
include(Config.cmake)

# enable cross-compiling: - should be called before run_vcpkg()
if(ENABLE_CROSS_COMPILING)
  enable_cross_compiler()
endif()

# install vcpkg dependencies: - should be called before defining project()
run_vcpkg()
run_conan()

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
# Binary Output Dir
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE_DIR}")
# Set the project name and language
project(
  cpp_core_project
  VERSION ${PROJECT_VERSION}
  DESCRIPTION ${PROJECT_DESCRIPTION}
  HOMEPAGE_URL ${PROJECT_WEBSITE}
  LANGUAGES CXX C
)

if(FEATURE_TESTS)
  # enable sanitizers and analyzers if running the tests
  # IMHO(CK) either:
  set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
  # XXX or: set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")
  set(ENABLE_COVERAGE "ENABLE_COVERAGE")
  set(ENABLE_VS_ANALYSIS "ENABLE_VS_ANALYSIS")

  check_sanitizers_support(
    ENABLE_SANITIZER_ADDRESS ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
    ENABLE_SANITIZER_LEAK ENABLE_SANITIZER_THREAD ENABLE_SANITIZER_MEMORY
  )

  # generate a main function for the test executable
  enable_testing()
  add_library(catch2_test_common INTERFACE)
  target_find_dependencies(catch2_test_common INTERFACE_CONFIG Catch2)
  target_link_libraries(
    catch2_test_common INTERFACE Catch2::Catch2 Catch2::Catch2WithMain
  )
  target_compile_definitions(catch2_test_common INTERFACE DO_NOT_USE_WMAIN)
  include(Catch)
endif()

if(FEATURE_DOCS)
  set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif()
include(enable_customized_project_options)
detect_architecture(arch)
# configure files based on CMake configuration options
add_subdirectory(configured_files)
include(gitVersion)
add_subdirectory(core)
if(ENABLE_SAMPLE_PROJECT_COMPILING)
  add_subdirectory(project)
endif()
