
file(GLOB MYFILES *.cpp)
list(FILTER MYFILES EXCLUDE REGEX "test_main.cpp")

foreach (MYFILE ${MYFILES})
  cmake_path(GET MYFILE STEM MYFILE_WITHOUT_EXT)
  message(STATUS ${MYFILE_WITHOUT_EXT})
  list(APPEND TEST_CASES ${MYFILE_WITHOUT_EXT})
endforeach ()

foreach (TEST_CASE ${TEST_CASES})
  add_executable(${TEST_CASE}
          ${TEST_CASE}.cpp)
  target_link_libraries(${TEST_CASE} PRIVATE doctest::doctest)
  target_compile_options(${TEST_CASE} PRIVATE ${compiler_options})
  target_compile_definitions(
          ${TEST_CASE} PRIVATE ${compiler_definitions}
          $<$<BOOL:${OPENSSL_FOUND}>:WITH_OPENSSL>)
  target_link_options(${TEST_CASE} PRIVATE ${linker_flags})

  target_include_directories(
          ${TEST_CASE}
          PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
          PRIVATE ../include

          # Any other extra include required
  )
  add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})
endforeach ()
