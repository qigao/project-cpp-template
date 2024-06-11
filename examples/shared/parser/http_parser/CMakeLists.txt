find_program(RAGEL 6.10)
ragel_target(
  HTTPParser http_parser.h ${CMAKE_CURRENT_BINARY_DIR}/httpParser.cpp
  COMPILE_FLAGS "-G2"
)

# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME HTTPParser)

file(GLOB TARGET_HEADERS *.h)
file(GLOB TARGET_SOURCES *.cpp)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files"
  FILES ${TARGET_SOURCES}
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files"
  FILES ${TARGET_HEADERS}
)

# ##############################################################################
# Define Target according to options
# ##############################################################################

add_library(
  ${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS}
                 ${RAGEL_HTTPParser_OUTPUTS}
)

# ##############################################################################
# Define Target Properties and Optional OpenSSL Dependency
# ##############################################################################

# generate an export header to expose symbols when using the library
generate_export_header(
  ${TARGET_NAME} EXPORT_FILE_NAME
  ${CMAKE_BINARY_DIR}/exports/${TARGET_NAME}_export.h
)

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

# add compiler definition WITH_OPENSSL, if found
target_compile_definitions(
  ${TARGET_NAME} PRIVATE ${compiler_definitions}
                         $<$<BOOL:${OPENSSL_FOUND}>:WITH_OPENSSL>
)

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
          # include OpenSSL directories if present
          $<$<BOOL:${OPENSSL_FOUND}>:${OPENSSL_INCLUDE_DIR}>
  # Any other extra include required
)

if(BUILD_BENCHMARKS)
  add_subdirectory(benchmarks/http_parser_ragel)
  add_subdirectory(benchmarks/http_parser_nodejs)
endif()
if(BUILD_TESTING)
  add_subdirectory(test)
endif()
