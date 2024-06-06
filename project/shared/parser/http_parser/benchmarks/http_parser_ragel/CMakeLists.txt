find_program(RAGEL 6.10)
ragel_target(
  HTTP_MACHINE ../../http_parser.rl ${CMAKE_CURRENT_BINARY_DIR}/http_parser.cpp COMPILE_FLAGS
  "-C -G2"
)
file(GLOB TARGET_HEADERS ../../*.h)

# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME ragel_http_parser_benchmark)
add_executable(${TARGET_NAME} ${RAGEL_HTTP_MACHINE_OUTPUTS} ragel_performance.cc)
target_link_libraries(${TARGET_NAME} benchmark::benchmark)

# add target properties such as debug postfix, MSVC Runtime and library version
set_target_properties(
  ${TARGET_NAME}
  PROPERTIES DEBUG_POSTFIX "-d"
             MSVC_RUNTIME_LIBRARY "${MSVC_RUNTIME_TYPE}"
             VERSION ${CMAKE_PROJECT_VERSION}
             SOVERSION ${CMAKE_PROJECT_VERSION}
)

# set target compile options as defined in the cmake/compiler_options.cmake
# Module
target_compile_options(${TARGET_NAME} PRIVATE ${compiler_options})

# add compiler definition
target_compile_definitions(${TARGET_NAME} PRIVATE ${compiler_definitions})

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME} PRIVATE {CMAKE_CURRENT_SOURCE_DIR} ../../gtest_helper ../../
  # Any other extra include required
)
