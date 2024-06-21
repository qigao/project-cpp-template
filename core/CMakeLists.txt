# ===============================================================================
add_subdirectory(http_client)
add_subdirectory(http_server)
add_subdirectory(shared)
# ===============================================================================
if(FEATURE_FUZZ_TESTS)
  add_subdirectory(../project/fuzz_test)
endif()
# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ../project/my_exe)
