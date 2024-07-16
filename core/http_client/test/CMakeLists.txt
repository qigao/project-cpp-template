add_test_config(http_client_lib SYSTEM_LIBRARIES http_client http_server_lib)

add_library_test(
  http_client
  yaml_convert
  CONFIGS
  catch2_test_common
  http_client_lib
  SOURCES
  yaml_convert_test.cpp
)

add_library_test(
  http_client
  sum
  CONFIGS
  catch2_test_common
  http_client_lib
  SOURCES
  sum_test.cpp
)

add_library_test(
  http_client
  yaml_config
  CONFIGS
  catch2_test_common
  http_client_lib
  SOURCES
  client_yml_test.cpp
)

add_library_test(
  http_client
  dll
  CONFIGS
  catch2_test_common
  http_client_lib
  SOURCES
  http_client_test.cpp
)
