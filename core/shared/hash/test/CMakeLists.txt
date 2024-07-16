add_executable(hash_function_test hash_function_test.cpp)
target_link_libraries(hash_function_test PRIVATE catch2_test_common hash_lib)
catch_discover_tests(
  hash_function_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
