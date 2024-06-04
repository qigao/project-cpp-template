find_package(nanobench)

add_test_config(my_lib SYSTEM_LIBRARIES nanobench::nanobench)

add_library_test(
  my_lib
  Singleton
  CONFIGS
  catch2_test_common
  SOURCES
  singleton_test.cpp
)
# add_library_test(
#   my_lib
#   hr
#   CONFIGS
#   catch2_test_common
#   my_lib
#   SOURCES
#   hr_test.cpp
# )
add_library_test(
  my_lib
  random
  CONFIGS
  catch2_test_common
  my_lib
  SOURCES
  random_test.cpp
)
add_library_test(
  my_lib
  auto_function
  CONFIGS
  catch2_test_common
  SOURCES
  auto_function_test.cpp
)
