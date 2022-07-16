set(TARGET_NAME URIParser_benchmark)

file(GLOB TARGET_SOURCES *.cpp)
file(GLOB TARGET_HEADERS ../*.h)

add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})
target_link_libraries(${TARGET_NAME} benchmark::benchmark URIParser)

# add target properties such as debug postfix, MSVC Runtime and library version
set_target_properties(
  ${TARGET_NAME}
  PROPERTIES DEBUG_POSTFIX "-d"
  MSVC_RUNTIME_LIBRARY "${MSVC_RUNTIME_TYPE}"
  VERSION ${CMAKE_PROJECT_VERSION}
  SOVERSION ${CMAKE_PROJECT_VERSION})

# set target compile options as defined in the cmake/compiler_options.cmake
# Module
target_compile_options(${TARGET_NAME} PRIVATE ${compiler_options})

# add compiler definition
target_compile_definitions(
  ${TARGET_NAME} PRIVATE ${compiler_definitions}
)

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# include all the necessary directories
target_include_directories(${TARGET_NAME}
    PRIVATE
	    ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC
	    ${DEFAULT_INCLUDE_DIRECTORIES}
)

