# ===============================================================================
add_subdirectory(my_header_lib)
add_subdirectory(my_lib)
add_subdirectory(my_exe)
add_subdirectory(http_client)
add_subdirectory(http_server)
add_subdirectory(async_demo)

# ===============================================================================
if(FEATURE_FUZZ_TESTS)
  add_subdirectory(fuzz_test)
endif()
# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT my_exe)
