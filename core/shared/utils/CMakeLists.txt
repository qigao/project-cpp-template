#add_subdirectory(test)
if(ENABLE_BENCHMARKS)
  add_subdirectory(benchmarks)

endif(ENABLE_BENCHMARKS)
# ----------------------------------------------------------------------------------------------------------------------
# tests
if(FEATURE_TESTS)
  add_subdirectory(test)
endif()
