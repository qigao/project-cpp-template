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
  if(ENABLE_MANUAL_SETUP)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    find_package(Catch2 PATHS ${EXTERNAL_LIBS_PATH}/catch2/lib/cmake/Catch2)
    find_package(
      yaml-cpp PATHS ${EXTERNAL_LIBS_PATH}/yaml-cpp/lib/cmake/yaml-cpp
    )
    find_package(yyjson PATHS ${EXTERNAL_LIBS_PATH}/yyjson/lib/cmake/yyjson)
  else()
    find_package(Catch2 3)
  endif()
  include(CTest)
  include(Catch)
  # Add handy test wrappers

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
add_subdirectory(cmake/configured_files)
