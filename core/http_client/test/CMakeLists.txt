find_package(GTest REQUIRED)
add_executable(http_client_test http_client_test.cpp)
target_link_libraries(
    http_client_test
    PRIVATE GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
            http_server_lib
            http_client
            starter_project_options
            starter_project_warnings
)

gtest_discover_tests(http_client_test)

add_executable(sum_test sum_test.cpp)
target_link_libraries(
    sum_test
    PRIVATE GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
            http_server_lib
            http_client
            starter_project_options
            starter_project_warnings
)
gtest_discover_tests(sum_test)

# --------------- YAML_PROPERTIES ---------------------
add_executable(
    client_yaml_test ../private/src/client_yaml.cpp client_yml_test.cpp
)
target_link_libraries(
    client_yaml_test
    PRIVATE GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
            http_client
            starter_project_options
            starter_project_warnings
)
gtest_discover_tests(client_yaml_test)

# --------------- YAML_CONVERT TEST -------------------
add_executable(client_yaml_convert_test yaml_convert_test.cpp)
target_link_libraries(
    client_yaml_convert_test
    PRIVATE GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
            http_client
            starter_project_options
            starter_project_warnings
)

gtest_discover_tests(client_yaml_convert_test)
