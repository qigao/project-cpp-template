# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME singleton_test)
find_package(nanobench)
find_package(GTest REQUIRED)

# ---------------- Singleton Benchmark TEST ---------------------
add_executable(${TARGET_NAME} singleton_test.cpp)
target_link_libraries(
  ${TARGET_NAME} PRIVATE GTest::gtest GTest::gtest_main nanobench::nanobench
                         starter_project_options starter_project_warnings
)
# include all the necessary directories
#target_include_directories(
#        ${TARGET_NAME}
#        PRIVATE {CMAKE_CURRENT_SOURCE_DIR} # Any other extra include required
#)
gtest_discover_tests(${TARGET_NAME})
