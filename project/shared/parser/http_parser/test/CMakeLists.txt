find_program(RAGEL 6.10)
ragel_target(
  HTTP_MACHINE ../http_parser.rl ${CMAKE_CURRENT_BINARY_DIR}/http_parser.cpp
  COMPILE_FLAGS "-C -G2"
)
file(GLOB TARGET_SOURCES *.cpp)
file(GLOB TARGET_HEADERS ../gtest_helper/*.h)
foreach(TARGET ${TARGET_SOURCES})
  cmake_path(GET TARGET STEM MYFILE_WITHOUT_EXT)
  list(APPEND TEST_CASES ${MYFILE_WITHOUT_EXT})
endforeach()

foreach(TEST_CASE ${TEST_CASES})
  add_executable(
    ${TEST_CASE} ${TEST_CASE}.cpp ${TARGET_HEADERS}
                 ${RAGEL_HTTP_MACHINE_OUTPUTS}
  )
  target_link_libraries(${TEST_CASE} PRIVATE GTest::gmock_main)

  target_compile_options(${TEST_CASE} PRIVATE ${compiler_options})
  target_compile_definitions(${TEST_CASE} PRIVATE ${compiler_definitions})

  target_link_options(${TEST_CASE} PRIVATE ${linker_flags})

  if(WIN32)
    target_link_libraries(${TEST_CASE} PRIVATE ws2_32)
  endif()

  target_include_directories(
    ${TEST_CASE} PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
    PRIVATE ../gtest_helper ../ # Any other extra include required
  )
  include(GoogleTest)
  gtest_discover_tests(${TEST_CASE})
  target_code_coverage(${TEST_CASE} AUTO)
endforeach()
