if(FEATURE_TESTS)
  add_subdirectory(test)
endif()
if(ENABLE_BENCHMARKS)
  add_subdirectory(benchmarks)

endif(ENABLE_BENCHMARKS)
