find_program(RAGEL 6.10)
ragel_target(
    CSV_Machine ../csv_record_parser.rl
    ${CMAKE_CURRENT_BINARY_DIR}/csv_record_parser.h COMPILE_FLAGS "-C -G2"
)
file(GLOB TARGET_SOURCES *.cc)
file(GLOB TARGET_HEADERS ../*.hpp ../*.h)

foreach(TARGET ${TARGET_SOURCES})
    cmake_path(GET TARGET STEM MYFILE_WITHOUT_EXT)
    list(APPEND TEST_CASES ${MYFILE_WITHOUT_EXT})
endforeach()

foreach(TEST_CASE ${TEST_CASES})
    add_executable(
        ${TEST_CASE} ${TEST_CASE}.cc ${TARGET_HEADERS}
                     ${RAGEL_CSV_Machine_OUTPUTS}
    )
    target_link_libraries(
        ${TEST_CASE} PUBLIC Threads::Threads PRIVATE GTest::gmock_main
                                                     CSVParser
    )
    target_compile_options(${TEST_CASE} PRIVATE ${compiler_options})
    target_compile_definitions(${TEST_CASE} PRIVATE ${compiler_definitions})

    target_link_options(${TEST_CASE} PRIVATE ${linker_flags})

    if(WIN32)
        target_link_libraries(${TEST_CASE} PRIVATE ws2_32)
    endif()

    target_include_directories(
        ${TEST_CASE} PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ../
                # Any other extra include required
    )
    include(GoogleTest)
    gtest_discover_tests(${TEST_CASE})
    target_code_coverage(${TEST_CASE} AUTO)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/
         DESTINATION ${CMAKE_BINARY_DIR}/bin/fixtures
    )
endforeach()
