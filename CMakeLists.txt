cmake_minimum_required(VERSION 3.21...3.28)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(fetch_project_options)

# Define the features of the project
include("./Features.cmake")

# enable cross-compiling: - should be called before run_vcpkg()
if(ENABLE_CROSS_COMPILING)
  enable_cross_compiler()
endif()

# install vcpkg dependencies: - should be called before defining project()
run_vcpkg()
run_conan()
# Set the project name and language
project(
  cpp_vcpkg_project
  DESCRIPTION "multi-module cpp project template for modern C++"
  HOMEPAGE_URL "https://github.com/aminya/cpp_vcpkg_project"
  LANGUAGES CXX C)
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
# Generate the compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# Configure the visibility of symbols 
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if(FEATURE_TESTS)
  # enable sanitizers and analyzers if running the tests
  # IMHO(CK) either:
  set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
  # XXX or: set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")
  set(ENABLE_COVERAGE "ENABLE_COVERAGE")
  set(ENABLE_VS_ANALYSIS "ENABLE_VS_ANALYSIS")

  check_sanitizers_support(
    ENABLE_SANITIZER_ADDRESS
    ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
    ENABLE_SANITIZER_LEAK
    ENABLE_SANITIZER_THREAD
    ENABLE_SANITIZER_MEMORY)

  # generate a main function for the test executable
  enable_testing()
  add_library(catch2_test_common INTERFACE)
  target_find_dependencies(catch2_test_common INTERFACE_CONFIG Catch2)
  target_link_libraries(catch2_test_common INTERFACE Catch2::Catch2 Catch2::Catch2WithMain)
  target_compile_definitions(catch2_test_common INTERFACE DO_NOT_USE_WMAIN)
  include(Catch)
endif()

if(FEATURE_DOCS)
  set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif()

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment to enable the options. Some of them accept one or more inputs:
project_options(
  PREFIX
  "my"
  ENABLE_CACHE
  ${ENABLE_CPPCHECK}
  ${ENABLE_CLANG_TIDY}
  ${ENABLE_VS_ANALYSIS}
  # ENABLE_INTERPROCEDURAL_OPTIMIZATION
  # ENABLE_NATIVE_OPTIMIZATION
  # ${ENABLE_DOXYGEN}
  ${ENABLE_COVERAGE}
  ${ENABLE_SANITIZER_ADDRESS}
  ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
  # ${ENABLE_SANITIZER_THREAD}
  # ${ENABLE_SANITIZER_MEMORY}
  # ENABLE_CONTROL_FLOW_PROTECTION
  # ENABLE_STACK_PROTECTION
  # ENABLE_OVERFLOW_PROTECTION
  # ENABLE_ELF_PROTECTION
  # ENABLE_RUNTIME_SYMBOLS_RESOLUTION
  # ENABLE_COMPILE_COMMANDS_SYMLINK
  # ENABLE_PCH
  # PCH_HEADERS
  # WARNINGS_AS_ERRORS
  # ENABLE_INCLUDE_WHAT_YOU_USE
  # ENABLE_GCC_ANALYZER
  # ENABLE_BUILD_WITH_TIME_TRACE
  # ENABLE_UNITY
  # LINKER "lld"
  # CONAN_PROFILE ${profile_path}
)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(gitversion)
add_subdirectory(core)
add_subdirectory(project)
# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT my_exe)
include(install)
include(PrintTargetInfo)