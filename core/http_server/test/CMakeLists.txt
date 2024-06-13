add_library_test(
  http_server_lib
  up_down
  CONFIGS
  common
  SOURCES
  tests.cpp
)

add_executable_test(http_server no_arg)
