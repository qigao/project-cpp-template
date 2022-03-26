file(GLOB MYFILES *.cpp)

foreach(MYFILE ${MYFILES})
  cmake_path(GET MYFILE STEM MYFILE_WITHOUT_EXT)
  list(APPEND TEST_CASES ${MYFILE_WITHOUT_EXT})
endforeach()

foreach(TEST_CASE ${TEST_CASES})
  add_executable(${TEST_CASE} ${TEST_CASE}.cpp ../include/singleton.hpp)
  target_link_libraries(${TEST_CASE}
    PUBLIC Threads::Threads
    PRIVATE doctest::doctest
  )

  target_compile_options(${TEST_CASE} PRIVATE ${compiler_options})
  target_compile_definitions(
    ${TEST_CASE} PRIVATE ${compiler_definitions}
  )

  target_link_options(${TEST_CASE} PRIVATE ${linker_flags})

  target_include_directories(
    ${TEST_CASE}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
    PRIVATE ../include

    # Any other extra include required
  )
  add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})
endforeach()
