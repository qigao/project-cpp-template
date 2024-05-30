## dependencies
set(DEPENDENCIES_CONFIGURED fmt Eigen3 docopt)

foreach(DEPENDENCY ${DEPENDENCIES_CONFIGURED})
  find_package(${DEPENDENCY} CONFIG REQUIRED)
endforeach()
add_subdirectory(./my_exe)
add_subdirectory(./my_lib)
add_subdirectory(./my_header_lib)
if(FEATURE_FUZZ_TESTS)
  add_subdirectory(fuzz_test)
endif()
