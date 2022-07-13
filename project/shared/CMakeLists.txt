if(BUILD_TESTING)
  add_subdirectory(utils/test)
  add_subdirectory(socket/test)
  add_subdirectory(csv_reader/test)
  add_subdirectory(uri_parser/test)
endif()

add_subdirectory(http_parser)

if(BUILD_BENCHMARKS)
  add_subdirectory(uri_parser/benchmarks)
endif()
