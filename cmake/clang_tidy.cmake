find_program(CLANG_TIDY_BIN NAMES clang-tidy)

if(CLANG_TIDY_BIN)
  message(STATUS "Found: clang-tidy")
  file(GLOB_RECURSE CPP_SOURCE_FILES *.cpp)
  file(GLOB_RECURSE CPP_HEADER_FILES *.h)

  add_custom_target(
    tidy-sources COMMAND ${CLANG_TIDY_BIN} -p ${PROJECT_SOURCE_DIR}
                         -checks=${CLANG_TIDY_CHECKS} ${CPP_SOURCE_FILES})
  add_custom_target(
    tidy-headers COMMAND ${CLANG_TIDY_BIN} -p ${PROJECT_SOURCE_DIR}
                         -checks=${CLANG_TIDY_CHECKS} ${CPP_HEADER_FILES})
  add_custom_target(
    tidy-all
    COMMENT "Run clang-tidy on all source files"
    DEPENDS tidy-sources tidy-headers)
endif()
