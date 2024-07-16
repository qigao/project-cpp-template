# - Customization point for project_options
# This module sets all customization on dynamic_project_options
#
# Include this module right after `project()` to make use
#
# If conan2 or vcpkg is requried, include(project_options) separately and invoke `run_conan2()` or `run_vcpkg()`
include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/options/Index.cmake)
# compile_commands.json
set(ENABLE_COMPILE_COMMANDS_SYMLINK_DEFAULT OFF)

# hardening
set(ENABLE_CONTROL_FLOW_PROTECTION_DEFAULT ON)
set(ENABLE_ELF_PROTECTION_DEFAULT OFF)
set(ENABLE_OVERFLOW_PROTECTION_DEFAULT ON)
set(ENABLE_RUNTIME_SYMBOLS_RESOLUTION_DEFAULT ON)
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(ENABLE_STACK_PROTECTION_DEFAULT OFF)
else()
  set(ENABLE_STACK_PROTECTION_DEFAULT ON)
endif()

# optimization
set(ENABLE_INTERPROCEDURAL_OPTIMIZATION_DEFAULT ON)
set(ENABLE_NATIVE_OPTIMIZATION_DEFAULT ON)

# install vcpkg/conan dependencies: - should be called before defining project()
run_conan()

# configure files based on CMake configuration options
project_options(
  PREFIX
  "starter"
  ENABLE_CACHE
  ${ENABLE_CPPCHECK}
  ${ENABLE_CLANG_TIDY}
  ${ENABLE_VS_ANALYSIS}
  # ENABLE_INTERPROCEDURAL_OPTIMIZATION
  # ENABLE_NATIVE_OPTIMIZATION
  # ${ENABLE_DOXYGEN}
  # ${ENABLE_COVERAGE}
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
