find_program(CPPCHECK_BIN NAMES cppcheck)

if(CPPCHECK_BIN)
  message(STATUS "Found: cppcheck")
  list(
    APPEND
    CMAKE_CXX_CPPCHECK
    "${CPPCHECK_BIN}"
    "--enable=all"
    "--enable=warning,performance,portability,information"
    "--inconclusive"
    "--check-config"
    "--force"
    "--quiet"
    "--inline-suppr"
    "--suppress=missingInclude"
    "--suppress=unmatchedSuppression"
    "--suppress=unusedFunction"
    "--template='{file}:{line}: warning: {id} ({severity}): {message}'")
endif()
