find_package(yyjson)
add_test_config(
  my_client SYSTEM_LIBRARIES yyjson::yyjson INCLUDES
  ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/http_client
)

add_library_test(
  http_client
  client_config
  WORKING_DIRECTORY
  ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}
  CONFIGS
  catch2_test_common
  my_client
  SOURCES
  client_config_test.cpp
)

add_library_test(
  http_client
  client_yml
  WORKING_DIRECTORY
  ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}
  CONFIGS
  catch2_test_common
  SOURCES
  ../private/src/client_yaml.cpp
  client_yml_test.cpp
)

add_library_test(
  http_client
  demo
  WORKING_DIRECTORY
  ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}
  CONFIGS
  catch2_test_common
  my_client
  SOURCES
  http_client_test.cpp
)
