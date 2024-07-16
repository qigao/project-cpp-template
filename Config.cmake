# building the tests
option(FEATURE_TESTS "Enable the tests" OFF)
if(FEATURE_TESTS)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

# building the docs
option(FEATURE_DOCS "Enable the docs" OFF)
option(ENABLE_OPENSSL "Enable OpenSSL support" OFF)
# fuzz tests
option(FEATURE_FUZZ_TESTS "Enable the fuzz tests" OFF)
option(ENABLE_BENCHMARKS "Enable benchmark tests" OFF)
option(ENABLE_CROSS_COMPILING "Detect cross compiler and setup toolchain" OFF)
option(ENABLE_SAMPLE_PROJECT_COMPILING "Enable sample projects" OFF)

if(FEATURE_TESTS)
  # enable sanitizers and analyzers if running the tests
  # IMHO(CK) either:
  set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
  # XXX or: set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")
  set(ENABLE_VS_ANALYSIS "ENABLE_VS_ANALYSIS")

  check_sanitizers_support(
    ENABLE_SANITIZER_ADDRESS ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
    ENABLE_SANITIZER_LEAK ENABLE_SANITIZER_THREAD ENABLE_SANITIZER_MEMORY
  )

  enable_testing()
  # code coverage
  if(${ENABLE_COVERAGE})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  endif()
  # Include CTest before any tests
  include(CTest)

  # Add handy test wrappers
  include(add_test)

  # Common test config
  add_test_config(
    catch2_test_common
    DEPENDENCIES_CONFIG
    Catch2
    LIBRARIES
    starter_project_options
    starter_project_warnings
    SYSTEM_LIBRARIES
    Catch2::Catch2WithMain
  )

endif()

if(FEATURE_DOCS)
  set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif()
